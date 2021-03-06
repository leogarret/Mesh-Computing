#include "mc_vcg_intersection.hpp"
#include <ctime>

#include <mc_vcg_obj_io.hpp>
#include <wrap\io_trimesh\export_obj.h>
#include <ctime>

#include "mc_geom.hpp"
#include <MTLogger-0.1.1\mtlogger\mtlogger.hpp>

using namespace mc;

/*
** Cette fonction permet de r�cuperer les coordonn�es d'un point d'intersection.
*/
TreeType::CoordType mc::geom::getPositionWithDistAndDir(TreeType::CoordType origin, TreeType::CoordType dir, TreeType::ScalarType dist)
{
	TreeType::CoordType intersectionPos;

	intersectionPos.X() = origin.X() + (dist * dir.X());
	intersectionPos.Y() = origin.Y() + (dist * dir.Y());
	intersectionPos.Z() = origin.Z() + (dist * dir.Z());

	return intersectionPos;
}

/*
** Cette fonction permet d'envoyer une ligne d'intersection sur un mesh.
** Elle retourne le nombre d'intersections.
*/
int intersection::Intersect(mc::mvcg::Mesh &m, IntersectionData &it, vcg::Ray3<TreeType::ScalarType> &ray)
{
	if (m.treeIsMake() == false) m.TreeMake();

	int nombreIntersections = 0;

	TreeType::ObjPtr faceIntersect;
	TreeType::ScalarType intersectionDist;
	vcg::RayTriangleIntersectionFunctor<> rayIntersector;
	const TreeType::ScalarType maxDist = std::numeric_limits<double>::max();

	std::vector<MCFace> faceBuff;

	int buff = -1;

	while ((faceIntersect = m.tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
	{
		if (faceIntersect->id == buff) break;

		nombreIntersections++;

		// On r�cup�re toutes les donn�es de l'intersection
		it.facesIntersections.push_back(*&faceIntersect);
		it.pointsIntersections.push_back(geom::getPositionWithDistAndDir(ray.Origin(), ray.Direction(), intersectionDist));
		it.objectTypes.push_back(intersection::onObjectType(*faceIntersect, it.pointsIntersections.at(it.pointsIntersections.size() - 1)));
		
		// On enregistre la face supprim�e
		faceBuff.push_back(*faceIntersect);
		buff = faceIntersect->id;

		int cnt = 0;
		for (auto& elem : m.face)
		{
			if (elem.id == buff)
			{
				m.face.erase(m.face.begin() + cnt);
			}
			++cnt;
		}
	}

	for (auto elem : faceBuff)
	{
		m.face.push_back(elem);
	}

	return nombreIntersections;
}

/* DEBUG */

void LaunchDebugIntersection(char *path)
{
	mvcg::Mesh m;
	mvcg::OpenObj(m, path);

	// Cr�ation de l'OcTree
	m.TreeMake();

	// On assigne une identifiant aux faces du mesh
	int idIdx = 0;
	for (auto& elem : m.face)
		elem.id = idIdx++;

	std::vector<intersection::IntersectionData> intersections;

	vcg::Ray3<TreeType::ScalarType> ray;
	std::vector<vcg::Ray3<TreeType::ScalarType>> rayList;

	std::clock_t start = std::clock();
	for (int i = -6; i <= 6; ++i)
	{
		for (int j = -6; j <= 6; ++j)
		{
			intersection::IntersectionData it; // Pour r�cuperer les infos sur les intersections

			// Position de la ligne d'intersection
			ray.SetOrigin(vcg::Point3d({ i * 10.0f - 1, j * 10.0f + 1, 200.0f }));
			ray.SetDirection(vcg::Point3d({ 0.0f, 0.0f, -10.0f }));
			rayList.push_back(ray);

			std::clock_t iStart = std::clock();
			intersection::Intersect(m, it, ray);
			std::clock_t iEnd = std::clock();

			// On ajoute les r�sultats de la ligne d'intersections � la liste
			intersections.push_back(it);
		}
	}
	std::clock_t end = std::clock();

	/************************ LOG DES RESULTATS *********************************/

	std::cout << "All intersections in " << end - start << "ms." << std::endl;

	int i = 0;
	for (auto elem : rayList)
	{
		mt::log(mt::mttrace, "Ligne\t%i ", i++);
		mt::log(mt::mttrace, "Origine(%f, %f, %f) - Direction(%f, %f, %f)\n",
			elem.Origin().X(), elem.Origin().Y(), elem.Origin().Z(), elem.Direction().X(), elem.Direction().Y(), elem.Direction().Z());
		mt::log(mt::mttrace, "\n");
	}

	mt::log(mt::mttrace, "\n==================================================================================================\n\n");
	mt::log(mt::mttrace, "Dur�e des %i intersections = %ims.\n\n", intersections.size(), end - start);
	mt::log(mt::mttrace, "==================================================================================================\n\n\n");

	i = 0;
	for (auto elem : intersections)
	{
		mt::log(mt::mttrace, "\n\nLigne\t%i Origine(%f, %f, %f) - Direction(%f, %f, %f)\n", i++,
			rayList[i].Origin().X(), rayList[i].Origin().Y(), rayList[i].Origin().Z(), rayList[i].Direction().X(), rayList[i].Direction().Y(), rayList[i].Direction().Z());
		mt::log(mt::mttrace, "--------------------------------------------------------------------------------------------------\n");

		int iInt = 0;
		for (auto item : elem.facesIntersections)
		{
			mt::log(mt::mttrace, "[%i] Intersection = [%i] -> (%.7f, %.7f, %.7f)\n", iInt, elem.objectTypes[iInt], elem.pointsIntersections[iInt].X(), elem.pointsIntersections[iInt].Y(), elem.pointsIntersections[iInt].Z());
			iInt++;
		}
	}

	mvcg::SaveObj(m, "OUTOBJ.obj");
}