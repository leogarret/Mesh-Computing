#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>
#include <mc_vcg_intersection.hpp>

// VCG
#include <wrap\io_trimesh\export_obj.h>

#include <signal.h>

// LOGGER
#include <logger.hpp>
using namespace logger;

//void debugIntersection();
OBJTYPE onObjectType(MCFace face, vcg::Point3d points);

#undef max

#endif // !_MAIN_C

void TestRay(mc::mvcg::Mesh &m)
{
	// Si l'octree n'est pas construit, on appelle la fonction de construction
	if (!m.treeIsMake())
		m.tree.Set(m.face.begin(), m.face.end());

	std::vector<MCFace> faceBuff;

	vcg::RayTriangleIntersectionFunctor<> rayIntersector;
	const MIndex::ScalarType maxDist = std::numeric_limits<double>::max();

	// Ray
	const MIndex::CoordType rayOrigin(-40, -20, 100);
	MIndex::CoordType rayDirection(0, 0, -100);

	const vcg::Ray3<MIndex::ScalarType, true> ray(rayOrigin, rayDirection);

	MIndex::ObjPtr isectFace;
	MIndex::ScalarType intersectionDist;
	MIndex::CoordType isectPt;

	std::cout << "Resultats de l'intersection:\n" << std::endl;

	std::clock_t start = std::clock();
	while ((isectFace = m.tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
	{
		isectPt = getPositionWithDistAndDir(rayOrigin, rayDirection, intersectionDist);
		printf("\tAdresse de la face -> 0x%p\n", isectFace);
		printf("\tDistance -> %f\n", intersectionDist);
		printf("\tIntersection(%f, %f, %f)\n\n", MIndex::ScalarType(isectPt.X()), MIndex::ScalarType(isectPt.Y()), MIndex::ScalarType(isectPt.Z()));
	
		std::cout << "Interect on " << onObjectType(*isectFace, isectPt) << std::endl;

		faceBuff.push_back(m.face[isectFace->id]);
		m.face.erase(m.face.begin() + isectFace->id);
	}
	
	// On remet les faces
	for (MCFace elem : faceBuff)
	{
		m.face.push_back(elem);
	}
	
	std::clock_t end = std::clock();
	std::cout << "Time = " << end - start << "ms." << std::endl;
	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "OUTOBJ.obj", 0);
}

int main()
{
	std::cout << "Hello World" << std::endl;
	//mc::mvcg::Mesh m;
	//mc::mvcg::obj::loader(m, "../../My Obj/ico_sphere.obj");
	//typedef typename decltype(m) Mesh_t;
	//
	//vcg::tri::UpdateTopology<Mesh_t>::VertexFace(m);
	//// Computing
	//vcg::tri::Smooth<Mesh_t>::VertexCoordPasoDoble(m, 1);

	//vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "ico_sphere_smooth.obj", 0);


	std::getchar();
	return 0;
}

