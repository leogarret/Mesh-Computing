#ifndef MC_VCG_EDGE_C
#define MC_VCG_EDGE_C

#include "mc_vcg_edge.hpp"

using namespace mc::mvcg::edge;

#endif // !MC_VCG_EDGE_C

Edge_t *mc::mvcg::edge::getEdges(MCFace face)
{
	Edge_t *edges = new Edge_t[3];

	edges[0].p1.X() = face.P(0)[0];
	edges[0].p1.Y() = face.P(0)[1];
	edges[0].p1.Z() = face.P(0)[2];
	edges[0].p2.X() = face.P(1)[0];
	edges[0].p2.Y() = face.P(1)[1];
	edges[0].p2.Z() = face.P(1)[2];

	edges[1].p1.X() = face.P(1)[0];
	edges[1].p1.Y() = face.P(1)[1];
	edges[1].p1.Z() = face.P(1)[2];
	edges[1].p2.X() = face.P(2)[0];
	edges[1].p2.Y() = face.P(2)[1];
	edges[1].p2.Z() = face.P(2)[2];

	edges[2].p1.X() = face.P(2)[0];
	edges[2].p1.Y() = face.P(2)[1];
	edges[2].p1.Z() = face.P(2)[2];
	edges[2].p2.X() = face.P(0)[0];
	edges[2].p2.Y() = face.P(0)[1];
	edges[2].p2.Z() = face.P(0)[2];

	return edges;
}

void mc::mvcg::edge::printFaceEdgeInfos(MCFace face)
{
	Edge_t *edges = getEdges(face);

	std::cout << "{ " << edges[0].p1[0] << " , " << edges[0].p1[1] << " , " << edges[0].p1[2] << " } --> ";
	std::cout << "{ " << edges[0].p2[0] << " , " << edges[0].p2[1] << " , " << edges[0].p2[2] << " }" << std::endl;

	std::cout << "{ " << edges[1].p1[0] << " , " << edges[1].p1[1] << " , " << edges[1].p1[2] << " } --> ";
	std::cout << "{ " << edges[1].p2[0] << " , " << edges[1].p2[1] << " , " << edges[1].p2[2] << " }" << std::endl;

	std::cout << "{ " << edges[2].p1[0] << " , " << edges[2].p1[1] << " , " << edges[2].p1[2] << " } --> ";
	std::cout << "{ " << edges[2].p2[0] << " , " << edges[2].p2[1] << " , " << edges[2].p2[2] << " }" << std::endl;
}
