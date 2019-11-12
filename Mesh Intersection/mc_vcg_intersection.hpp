#ifndef MC_VCG_INTERSECTION_HPP
#define MC_VCG_INTERSECTION_HPP

#include <vcg/complex/complex.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <vcg/complex/algorithms/create/platonic.h>

#include <mc_vcg_mesh.hpp>

#include <ctime>

/*
** Types d'objets utilisés pour déterminer les intersections.
*/

struct BuffIntersect;

namespace mc
{
	enum OBJTYPE {
		T_FACE = 0,
		T_EDGE = 1,
		T_POINT = 2
	};

	typedef vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass> MIndex;

	MIndex::CoordType getPositionWithDistAndDir(MIndex::CoordType origin, MIndex::CoordType dir, MIndex::ScalarType dist);

	struct BuffIntersect {
		int nbIntersections;
		std::vector<MCFace*> facesIntersections;
		std::vector<MIndex::CoordType> pointsIntersections;
		vcg::Point3d origin;
		vcg::Point3d direction;
		std::clock_t time;
	};

	int Intersect(mc::mvcg::Mesh &m, BuffIntersect it);
}

#endif //!MC_VCG_INTERSECTION_HPP