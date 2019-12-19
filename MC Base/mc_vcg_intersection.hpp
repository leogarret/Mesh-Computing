#ifndef MC_VCG_INTERSECTION_HPP
#define MC_VCG_INTERSECTION_HPP

#include <vcg/complex/complex.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <vcg/complex/algorithms/create/platonic.h>

#include <mc_vcg_mesh.hpp>
#include "mc_vcg_intersectionData.hpp"
#include "mc_all_intersection.hpp"

#include <ctime>

#ifndef TreeType
#define TreeType vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass>
#endif // !TreeType

using namespace mc::mvcg;
using namespace mc::mvcg::intersection;

namespace mc::mvcg::intersection
{
	
	int Intersect(mc::mvcg::Mesh &m, IntersectionData &it, vcg::Ray3<TreeType::ScalarType> &ray);

	OBJTYPE onObjectType(MCFace face, vcg::Point3d points);

}
#endif //!MC_VCG_INTERSECTION_HPP