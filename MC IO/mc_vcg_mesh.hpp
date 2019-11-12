#pragma once
#ifndef MC_VCG_MESH_H
#define MC_VCG_MESH_H

#include <vcg/complex/complex.h>
#include <wrap\io_trimesh\import_obj.h>
#include <wrap\io_trimesh\import_stl.h>
#include <wrap/callback.h>
#include <vcg/simplex/face/component_ep.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>

class MCVertex; class MCEdge; class MCFace;

struct MyUsedTypes : public vcg::UsedTypes<vcg::Use<MCVertex>::AsVertexType,
	vcg::Use<MCEdge>::AsEdgeType,
	vcg::Use<MCFace>::AsFaceType> {};

class MCVertex : public vcg::Vertex<MyUsedTypes, vcg::vertex::Coord3d, vcg::vertex::Normal3d, vcg::vertex::BitFlags, vcg::vertex::VFAdj, vcg::vertex::Qualityf> {};
class MCFace : public vcg::Face<MyUsedTypes, vcg::face::FFAdj, vcg::face::VertexRef, vcg::face::BitFlags, vcg::face::VFAdj, vcg::face::Normal3d, vcg::face::Mark, vcg::face::EdgePlane, vcg::face::EFAdj>
{
public:int id = 0;
};
class MCEdge : public vcg::Edge < MyUsedTypes, vcg::edge::EEAdj, vcg::edge::VertexRef, vcg::face::EdgePlane, vcg::edge::BitFlags > {};

namespace mc::mvcg {

	class MeshInfo;

	class Mesh : public vcg::tri::TriMesh<std::vector<MCVertex>, std::vector<MCFace>, std::vector<MCEdge>>
	{
	public:
		Mesh() {};
		inline void TreeMake() { tree.Set(face.begin(), face.end()); this->_treeIsMake = true; };

		mc::mvcg::MeshInfo *infos;
		vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info_buff;

		inline bool treeIsMake() { return this->_treeIsMake; };

		vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass> tree;
	private: bool _treeIsMake = false;
	};

	class MeshInfo
	{
	public:
		/***** OBJ *****/
		MeshInfo(vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::Info info);

		/***** STL *****/
		MeshInfo(mc::mvcg::Mesh &mesh);

		/***** GENERAL *****/
		int mask = 0;
		vcg::CallBackPos *cb;
		int numVertices = 0;
		int numEdges = 0;
		int numFaces = 0;
		int numTexCoords = 0;
		int numNormals = 0;

	};

};

#endif // !MC_VCG_MESH_H