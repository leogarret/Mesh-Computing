#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>
#include <mc_vcg_intersection.hpp>

// VCG
#include <wrap\io_trimesh\export_obj.h>

// LOGGER
#include <logger.hpp>
using namespace logger;

void debug_intersection_v2(char *path);
OBJTYPE onObjectType(MyFace face, vcg::Point3d points);

typedef struct {
	MyFace face;
	int position;
} faceBuffer;

#endif // !_MAIN_C

void TestRay(mc::mvcg::Mesh &m)
{
	// Si l'octree n'est pas construit, on appelle la fonction de construction
	if (!m.treeIsMake())
		m.tree.Set(m.face.begin(), m.face.end());

	std::vector<faceBuffer> faceBuff;

	vcg::RayTriangleIntersectionFunctor<> rayIntersector;
	const MIndex::ScalarType maxDist = std::numeric_limits<double>::max();

	// Ray
	const MIndex::CoordType rayOrigin(-1, -1, 100);
	MIndex::CoordType rayDirection(0, 0, -100);

	const vcg::Ray3<MIndex::ScalarType, true> ray(rayOrigin, rayDirection);
	rayDirection.Norm();

	MIndex::ObjPtr isectFace;
	MIndex::ScalarType intersectionDist;
	MIndex::CoordType isectPt;

	std::cout << "Résultats de l'intersection:\n" << std::endl;
	while ((isectFace = m.tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
	{
		isectPt = getPositionWithDistAndDir(rayOrigin, rayDirection, intersectionDist);
		printf("\tAdresse de la face -> 0x%p\n", isectFace);
		printf("\tDistance -> %f\n", intersectionDist);
		printf("\tIntersection(%f, %f, %f)\n\n", MIndex::ScalarType(isectPt.X()), MIndex::ScalarType(isectPt.Y()), MIndex::ScalarType(isectPt.Z()));
		
		std::cout << "Interect on " << onObjectType(*isectFace, isectPt) << std::endl;

		// On retire la face temporairement
		int cnt = 0;
		for (; &m.face[cnt] != isectFace; ++cnt);
		faceBuff.push_back({ m.face[cnt], cnt });
		m.face.erase(m.face.begin() + cnt);
	}

	// On remet les faces
	for (auto elem : faceBuff)
	{
		m.face.insert(m.face.begin() + elem.position, elem.face);
	}
	
	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "OUTOBJ.obj", 0);
}

int main()
{
	mc::mvcg::Mesh mesh;
	mc::mvcg::obj::loader(mesh, "../../obj/Sphere-632586.obj");
	mesh.TreeMake();
	TestRay(mesh);
	
	std::getchar();
	return 0;
}