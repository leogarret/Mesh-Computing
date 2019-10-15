#pragma once
#ifndef MC_VCG_MESH_H
#define MC_VCG_MESH_H

#include<vcg/complex/complex.h>
#include <wrap\io_trimesh\import_obj.h>
#include <wrap\io_trimesh\import_stl.h>
#include <wrap/callback.h>
#include <vcg/simplex/face/component_ep.h>

class MyVertex; class MyEdge; class MyFace;

struct MyUsedTypes : public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType,
	vcg::Use<MyEdge>::AsEdgeType,
	vcg::Use<MyFace>::AsFaceType> {};

class MyVertex : public vcg::Vertex<MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags, vcg::vertex::VFAdj, vcg::vertex::Qualityf> {};
class MyFace : public vcg::Face<MyUsedTypes, vcg::face::FFAdj, vcg::face::VertexRef, vcg::face::BitFlags, vcg::face::VFAdj, vcg::face::Normal3f, vcg::face::Mark, vcg::face::EdgePlane> {};
class MyEdge : public vcg::Edge < MyUsedTypes, vcg::edge::EEAdj > {};

class MyEdgeMesh : public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyEdge> > {};

namespace mc::mvcg {

	class MeshInfo;

	class Mesh : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>, std::vector<MyEdge>>
	{
	public:mc::mvcg::MeshInfo *infos;

	public:vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info_buff;
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