#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>

// VCG
#include<vcg/complex/complex.h>
#include<vcg/simplex/face/distance.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/component_ep.h>
#include <vcg/complex/algorithms/update/flag.h>
#include <vcg/space/intersection3.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <wrap\io_trimesh\export_obj.h>

// LOGGER
#include <logger.hpp>
using namespace logger;

#undef max;

typedef float MScalarType;

typedef vcg::AABBBinaryTreeIndex<MyFace, MScalarType, vcg::EmptyClass> MIndex;

MIndex tree;

#endif // !_MAIN_C

void TestClosest()
{
	mc::mvcg::Mesh m;
	mc::mvcg::obj::loader(m, "../../obj/cube.obj");

	vcg::tri::UpdateNormal<mc::mvcg::Mesh>::PerVertexNormalized(m);
	vcg::tri::UpdateComponentEP<mc::mvcg::Mesh>::Set(m);
	tree.Set(m.face.begin(), m.face.end());

	vcg::face::PointDistanceEPFunctor<MIndex::ScalarType> getPtDist;
	const MIndex::CoordType queryPoint(0, 0, 2);
	const MIndex::ScalarType maxDist = std::numeric_limits<MIndex::ScalarType>::max();

	MIndex::ObjPtr closestFace;
	MIndex::ScalarType closestDist;
	MIndex::CoordType closestPoint;

	closestFace = tree.GetClosest(getPtDist, vcg::EmptyClass{}, queryPoint, maxDist, closestDist, closestPoint);

	printf("Informations sur l'intersection:\n");

	if (closestFace != 0) {
		printf("\tface     : 0x%p\n", closestFace);
		printf("\tdistance : %f\n", closestDist);
		printf("\tpoint    : [%f, %f, %f]\n", closestPoint[0], closestPoint[1], closestPoint[2]);
	}
	else {
		printf("\tno object found (index is probably empty).\n");
	}

	int cnt = 0;
	for (; &m.face[cnt] != closestFace; ++cnt);

	std::cout << cnt << std::endl;
	m.face.erase(m.face.begin() + cnt);

	std::getchar();
	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "cube_without_face.obj", 0);
}


void singleIntersect()
{
	mc::mvcg::Mesh m;
	mc::mvcg::obj::loader(m, "../../obj/cube.obj");

	vcg::tri::UpdateNormal<mc::mvcg::Mesh>::PerVertexNormalized(m);
	vcg::tri::UpdateComponentEP<mc::mvcg::Mesh>::Set(m);
	tree.Set(m.face.begin(), m.face.end());

	const bool TEST_BACK_FACES = true;

	vcg::RayTriangleIntersectionFunctor<TEST_BACK_FACES> rayIntersector;
	const MIndex::ScalarType maxDist = std::numeric_limits<MIndex::ScalarType>::max();
	const MIndex::CoordType rayOrigin((MIndex::ScalarType)0, (MIndex::ScalarType)0, (MIndex::ScalarType)0);
	const MIndex::CoordType rayDirection((MIndex::ScalarType)1, (MIndex::ScalarType)0, (MIndex::ScalarType)0);
	const vcg::Ray3<MIndex::ScalarType, false> ray(rayOrigin, rayDirection);

	MIndex::ObjPtr isectFace;
	MIndex::ScalarType rayT;
	MIndex::CoordType isectPt;

	vcg::EmptyClass a;
	isectFace = tree.DoRay(rayIntersector, a, ray, maxDist, rayT);
}

void multipleIntersect()
{
	mc::mvcg::Mesh m;
	mc::mvcg::obj::loader(m, "../../obj/cube.obj");

	vcg::tri::UpdateNormal<mc::mvcg::Mesh>::PerVertexNormalized(m);
	vcg::tri::UpdateComponentEP<mc::mvcg::Mesh>::Set(m);

	tree.Set(m.face.begin(), m.face.end());

	vcg::face::PointDistanceEPFunctor<MIndex::ScalarType> getPtDist;

	const unsigned int k = 15;

	const MIndex::CoordType queryPoint((MIndex::ScalarType)0, (MIndex::ScalarType)0, (MIndex::ScalarType)0);
	const MIndex::ScalarType maxDist = std::numeric_limits<MIndex::ScalarType>::max();

	std::vector<MIndex::ObjPtr> closestObjects;
	std::vector<MIndex::ScalarType> closestDistances;
	std::vector<MIndex::CoordType> closestPoints;

	vcg::EmptyClass a;
	unsigned int rk = tree.GetKClosest(getPtDist, a, k, queryPoint, maxDist, closestObjects, closestDistances, closestPoints);

	std::cout << "**** Informations sur les intersections ****" << std::endl;

	for (int i = 0; i <= 10; ++i) {
		std::cout << "Adresse de la face : 0x" << closestObjects[i] << std::endl;
		std::cout << "Distance : " << closestDistances[i] << std::endl;
		std::cout << "Coordonnees : " << closestPoints[i].X() << ", " << closestPoints[i].Y() << ", " << closestPoints[i].Z() << std::endl;
		std::cout << "*************************************\n" << std::endl;
	}
}

int main()
{
	TestClosest();
	getchar();
	return 0;
}