#ifndef MC_VCG_EDGE_H
#define MC_VCG_EDGE_H

#include<vcg/complex/complex.h>

#include "mc_vcg_mesh.hpp"

namespace mc::mvcg::edge {

	struct Edge_t {
		vcg::Point3f p1 = { 0, 0, 0 };
		vcg::Point3f p2 = { 0, 0, 0 };
	};

	Edge_t *getEdges(MCFace face);
	void printFaceEdgeInfos(MCFace face);
}

#endif // !MC_VCG_EDGE_H