#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>

// VCG
#include <wrap\io_trimesh\export_obj.h>
#include <vcg/complex/algorithms/intersection.h>

#include <vcg/space/distance3.h>
#include <vcg/space/index/aabb_binary_tree/ray.h>
#include <vcg/space/index/aabb_binary_tree/base.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>

#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/update/bounding.h>

// LOGGER
#include <logger.hpp>
using namespace logger;
using namespace vcg;

typedef vcg::AABBBinaryTree<mc::mvcg::Mesh, mc::mvcg::Mesh::ScalarType, int> tType;

#endif // !_MAIN_C

int main()
{
	mc::mvcg::Mesh m;
	mc::mvcg::primitives::createCube(m);

	vcg::AABBBinaryTreeRay<tType> tRay;

start:static int i = 0;
	std::cout << i << std::endl;
if (!(++i <= 5)) goto end;goto start;end:

	getchar();
	return 0;
}