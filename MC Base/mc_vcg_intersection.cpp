#include "mc_vcg_intersection.hpp"
#include <ctime>

#include <mc_vcg_obj_importer.hpp>
#include <wrap\io_trimesh\export_obj.h>
#include <ctime>

#include <MTLogger-0.1.1\mtlogger\mtlogger.hpp>

#undef max

typedef double MScalarType;
typedef vcg::AABBBinaryTreeIndex<MCFace, MScalarType, vcg::EmptyClass> MIndex;

using namespace mc;

/*
** Cette fonction permet de récuperer les coordonnées d'un point d'intersection.
*/
mc::MIndex::CoordType mc::getPositionWithDistAndDir(MIndex::CoordType origin, MIndex::CoordType dir, MIndex::ScalarType dist)
{
	MIndex::CoordType intersectionPos;

	intersectionPos.X() = origin.X() + (dist * dir.X());
	intersectionPos.Y() = origin.Y() + (dist * dir.Y());
	intersectionPos.Z() = origin.Z() + (dist * dir.Z());

	return intersectionPos;
}

/*
** Cette fonction permet d'envoyer une ligne d'intersection sur un mesh.
*/
int mc::Intersect(mc::mvcg::Mesh &m, BuffIntersect &it, vcg::Ray3<MIndex::ScalarType> &ray)
{
	int nombreIntersections = 0;

	MIndex::ObjPtr isectFace;
	MIndex::ScalarType intersectionDist;
	vcg::RayTriangleIntersectionFunctor<> rayIntersector;
	const MIndex::ScalarType maxDist = std::numeric_limits<double>::max();

	std::vector<MCFace> faceBuff;

	int buff = -1;

	while ((isectFace = m.tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
	{
		if (isectFace->id == buff) break;

		// On récupère toutes les données de l'intersection
		nombreIntersections++;
		it.facesIntersections.push_back(*&isectFace);
		it.pointsIntersections.push_back(getPositionWithDistAndDir(it.origin, it.direction, intersectionDist));
		it.objectTypes.push_back(mc::onObjectType(*isectFace, it.pointsIntersections.at(it.pointsIntersections.size() - 1)));

		// On enregistre la face supprimée
		faceBuff.push_back(*isectFace);
		buff = isectFace->id;

		int cnt = 0;
		for (auto& elem : m.face)
		{
			if (elem.id == buff)
			{
				m.face.erase(m.face.begin() + cnt);
				break;
			}
			++cnt;
		}
	}
	mt::log(stdout, "\n");

	for (auto elem : faceBuff)
	{
		m.face.push_back(elem);
	}

	it.nbIntersections = nombreIntersections;
	return nombreIntersections;
}

/* DEBUG */

void mc::LaunchDebugIntersection(char *path)
{
	mc::mvcg::Mesh m;
	mc::mvcg::obj::loader(m, path);

	// Création de l'OcTree
	m.TreeMake();

	// On assigne une identifiant aux faces du mesh
	int idIdx = 0;
	for (auto& elem : m.face)
		elem.id = idIdx++;

	std::vector<mc::BuffIntersect> intersections;

	vcg::Ray3<mc::MIndex::ScalarType> ray;

	std::clock_t start = std::clock();
	for (int i = -6; i <= 6; ++i)
	{
		for (int j = -6; j <= 6; ++j)
		{
			mc::BuffIntersect it; // Pour récuperer les infos sur les intersections

			// Position de la ligne d'intersection
			it.origin = vcg::Point3d({ i * 10.0f, j * 10.0f, 200.0f });
			it.direction = vcg::Point3d({ 0.0f, 0.0f, -10.0f });
			ray.Set(it.origin, it.direction);
			it.nbIntersections = 0;

			std::clock_t iStart = std::clock();
			it.nbIntersections = mc::Intersect(m, it, ray);
			std::clock_t iEnd = std::clock();
			it.time = iEnd - iStart;

			// On ajoute les résultats de la ligne d'intersections à la liste
			intersections.push_back(it);
		}
	}
	std::clock_t end = std::clock();

	/************************ LOG DES RESULTATS *********************************/

	std::cout << "All intersections in " << end - start << "ms." << std::endl;

	int i = 0;
	for (auto elem : intersections)
	{
		mt::log(mt::mttrace, "Ligne\t%i ", i++);
		mt::log(mt::mttrace, "Origine(%f, %f, %f) - Direction(%f, %f, %f)\n",
			elem.origin[0], elem.origin[1], elem.origin[2], elem.direction[0], elem.direction[1], elem.direction[2]);
		mt::log(mt::mttrace, "\n");
	}

	mt::log(mt::mttrace, "\n==================================================================================================\n\n");
	mt::log(mt::mttrace, "Durée des %i intersections = %ims.\n\n", intersections.size(), end - start);
	mt::log(mt::mttrace, "==================================================================================================\n\n\n");
	
	i = 0;
	for (auto elem : intersections)
	{
		mt::log(mt::mttrace, "\n\nLigne\t%i Origine(%f, %f, %f) - Direction(%f, %f, %f)\n", i++,
			elem.origin[0], elem.origin[1], elem.origin[2], elem.direction[0], elem.direction[1], elem.direction[2]);
		mt::log(mt::mttrace, "--------------------------------------------------------------------------------------------------\n");
		mt::log(mt::mttrace, "Nb. Int = %i -> %ims\n\n", elem.nbIntersections, elem.time);

		for (int iInt = 0; elem.nbIntersections > 0 && iInt < elem.nbIntersections; ++iInt)
		{
			mt::log(mt::mttrace, "[%i] Intersection = [%i] -> (%.7f, %.7f, %.7f)\n", iInt, elem.objectTypes[iInt], elem.pointsIntersections[iInt].X(), elem.pointsIntersections[iInt].Y(), elem.pointsIntersections[iInt].Z());
		}
	}

	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "OUTOBJ.obj", 0);
}