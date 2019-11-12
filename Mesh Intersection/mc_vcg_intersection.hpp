#ifndef MC_VCG_INTERSECTION_HPP
#define MC_VCG_INTERSECTION_HPP

#include <vcg/complex/complex.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <vcg/complex/algorithms/create/platonic.h>

#include <mc_vcg_mesh.hpp>

/*
** Types d'objets utilisés pour déterminer les intersections.
*/
enum OBJTYPE {
	T_FACE = 0,
	T_EDGE = 1,
	T_POINT = 2
};

struct BuffIntersect;

typedef vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass> MIndex;

//void debug_intesections();


int Intersect(mc::mvcg::Mesh &m, BuffIntersect it);
MIndex::CoordType getPositionWithDistAndDir(MIndex::CoordType origin, MIndex::CoordType dir, MIndex::ScalarType dist);

#endif //!MC_VCG_INTERSECTION_HPP