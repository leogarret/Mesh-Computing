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

namespace mc::geom
{
	TreeType::CoordType getPositionWithDistAndDir(TreeType::CoordType origin, TreeType::CoordType dir, TreeType::ScalarType dist);

	double getPointEdgeDistance(vcg::Point3d edge[2], vcg::Point3d p);

	vcg::Point3d getProjection(vcg::Point3d edge[2], vcg::Point3d p);

	double getPointsDistance(vcg::Point3d p1, vcg::Point3d p2);

}
#endif //!MC_VCG_INTERSECTION_HPP