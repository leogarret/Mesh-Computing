#ifndef MC_VCG_INTERSECTION_HPP
#define MC_VCG_INTERSECTION_HPP

#include <vcg/complex/complex.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <vcg/complex/algorithms/create/platonic.h>

#include <mc_vcg_mesh.hpp>

/*
** Types d'objets utilisés pour déterminer les intersections.
*/
typedef enum OBJTYPE {
	T_FACE = 0,
	T_EDGE = 1,
	T_POINT = 2
};

#undef max;
typedef vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass> MIndex;

void debug_intesections();

int Intersect(std::vector<MCFace> mf, MIndex &tree, std::vector<MCFace*> &faces, std::vector<MIndex::CoordType> &points, vcg::Point3d origin, vcg::Point3d direction);
MIndex::CoordType getPositionWithDistAndDir(MIndex::CoordType origin, MIndex::CoordType dir, MIndex::ScalarType dist);

#endif //!MC_VCG_INTERSECTION_HPP