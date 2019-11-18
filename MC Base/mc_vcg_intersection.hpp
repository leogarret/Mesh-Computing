#ifndef MC_VCG_INTERSECTION_HPP
#define MC_VCG_INTERSECTION_HPP

#include <vcg/complex/complex.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <vcg/complex/algorithms/create/platonic.h>

#include <mc_vcg_mesh.hpp>

#include <ctime>

struct BuffIntersect;

namespace mc
{
	/*
	** Type d'objet pour déterminer l'objet en intersection.
	*/
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
		std::vector<int> objectTypes;
	};

	int Intersect(mc::mvcg::Mesh &m, BuffIntersect &it, vcg::Ray3<MIndex::ScalarType> &ray);
	OBJTYPE onObjectType(MCFace face, vcg::Point3d points);

	/* UTILS */
	double getPointEdgeDistance(vcg::Point3d edge[2], vcg::Point3d p);

	/* DEBUG */
	void LaunchDebugIntersection(char *path);
}

#endif //!MC_VCG_INTERSECTION_HPP