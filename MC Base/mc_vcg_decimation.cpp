#include <mc_vcg_mesh.hpp>
#include <stdexcept>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>

using namespace mc::mvcg;

typedef vcg::tri::BasicVertexPair<MCVertex> VertexPair;

class MyTriEdgeCollapse : public vcg::tri::TriEdgeCollapseQuadric<Mesh, VertexPair, MyTriEdgeCollapse, vcg::tri::QInfoStandard<MCVertex> >
{
public:
	typedef vcg::tri::TriEdgeCollapseQuadric<Mesh, VertexPair, MyTriEdgeCollapse, vcg::tri::QInfoStandard<MCVertex> > TECQ;
	typedef  Mesh::VertexType::EdgeType EdgeType;
	inline MyTriEdgeCollapse(const VertexPair &p, int i, vcg::BaseParameterClass *pp) : TECQ(p, i, pp) {}
};


/*
** Cette méthode permet de décimer un maillage.
*/
void Mesh::decimate(int targetFaces, bool preserveBoundary)
{
	if (preserveBoundary)
	{
		Mesh::VertexIterator vi;
		int bv = 0;
		for (vi = this->vert.begin(); vi != this->vert.end(); ++vi)
		{
			if (vi->IsD()) continue;
			if (vi->IsB()) ++bv;
		}
		targetFaces += bv;
	}

	if (targetFaces > this->FN()) return;

	vcg::tri::TriEdgeCollapseQuadricParameter params;
	params.PreserveBoundary = preserveBoundary;
	params.BoundaryWeight = 999;

	vcg::LocalOptimization<Mesh> collapse(*this, &params);
	collapse.Init<MyTriEdgeCollapse>();

	collapse.SetTargetSimplices(targetFaces);
	collapse.SetTimeBudget(0.5f);

	while (collapse.DoOptimization() && this->FN() > targetFaces){};

	collapse.Finalize<MyTriEdgeCollapse>();

	vcg::tri::Allocator<Mesh>::CompactFaceVector(*this);
	vcg::tri::Allocator<Mesh>::CompactVertexVector(*this);
}