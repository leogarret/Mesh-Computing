#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>

// VCG
#include <wrap\io_trimesh\export_obj.h>
#include <stdio.h>
#include<vcg/complex/complex.h>
#include<vcg/simplex/face/distance.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/component_ep.h>
#include <vcg/complex/algorithms/update/flag.h>
#include <vcg/space/intersection3.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>

// LOGGER
#include <logger.hpp>
using namespace logger;
using namespace vcg;

#undef max;

typedef float MScalarType;

typedef vcg::AABBBinaryTreeIndex<MyFace, MScalarType, vcg::EmptyClass> MIndex;

MIndex gIndex;

#endif // !_MAIN_C

int main()
{
	mc::mvcg::Mesh m;
	vcg::tri::Dodecahedron<mc::mvcg::Mesh>(m);

	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "outDode.obj", 0);

	vcg::tri::UpdateFlags<mc::mvcg::Mesh>::Clear(m);
	vcg::tri::UpdateNormal<mc::mvcg::Mesh>::PerVertexNormalized(m);
	vcg::tri::UpdateComponentEP<mc::mvcg::Mesh>::Set(m);

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
	isectFace = gIndex.DoRay(rayIntersector, a, ray, maxDist, rayT);

	printf("DoRay Test:\n");
	if (isectFace != 0)
	{
		printf("\tface  : 0x%p\n", isectFace);
		printf("\tray t : %f\n", rayT);
	}
	else
	{
		printf("\tno object found (index is probably empty).\n");
	}


	getchar();

	return 0;
}