#ifndef MC_VCG_MESH_H
#define MC_VCG_MESH_H

#include<vcg/complex/complex.h>
#include <wrap\io_trimesh\import_obj.h>
#include <wrap\io_trimesh/export_obj.h>

class MyVertex; class MyEdge; class MyFace;

struct MyUsedTypes : public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType,
	vcg::Use<MyEdge>::AsEdgeType,
	vcg::Use<MyFace>::AsFaceType> {};

class MyVertex : public vcg::Vertex<MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags> {};
class MyFace : public vcg::Face<MyUsedTypes, vcg::face::FFAdj, vcg::face::VertexRef, vcg::face::BitFlags> {};
class MyEdge : public vcg::Edge<MyUsedTypes> {};

namespace mc::mvcg {

	class Mesh : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>, std::vector<MyEdge>>
	{
	public: vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::Info info;
	};

}

#endif // !MC_VCG_MESH_H