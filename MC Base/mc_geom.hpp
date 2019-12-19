#ifndef MC_GEOM_HPP
#define MC_GEOM_HPP

#include <vcg/complex/complex.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <vcg/complex/algorithms/create/platonic.h>

#include <mc_vcg_mesh.hpp>

#ifndef TreeType
#define TreeType vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass>
#endif // !TreeType

namespace mc::geom
{
	TreeType::CoordType getPositionWithDistAndDir(TreeType::CoordType origin, TreeType::CoordType dir, TreeType::ScalarType dist);

	double getPointEdgeDistance(vcg::Point3d edge[2], vcg::Point3d p);

	vcg::Point3d getProjection(vcg::Point3d edge[2], vcg::Point3d p);

	double getPointsDistance(vcg::Point3d p1, vcg::Point3d p2);

}

#endif //!MC_GEOM_HPP