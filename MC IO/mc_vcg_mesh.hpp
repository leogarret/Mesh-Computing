#pragma once
#ifndef MC_VCG_MESH_H
#define MC_VCG_MESH_H

#include <vcg/complex/complex.h>
#include <vcg/simplex/face/component_ep.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>
#include <vcg/math/quadric.h>

#include <wrap\io_trimesh\import_obj.h>
#include <wrap\io_trimesh\export_obj.h>
#include <wrap/io_trimesh/import_stl.h>
#include <wrap/io_trimesh/export_stl.h>
#include <wrap/callback.h>

class MCVertex; class MCEdge; class MCFace;

// Cette structure définie les classes qui vont être utilisées pour les faces, sommets et arêtes.
struct MyUsedTypes :
	public vcg::UsedTypes<vcg::Use<MCVertex>::AsVertexType,
	vcg::Use<MCEdge>::AsEdgeType,
	vcg::Use<MCFace>::AsFaceType> {};

// Cette class est utilisée pour définir un sommet
class MCVertex :
	public vcg::Vertex<MyUsedTypes,
	vcg::vertex::Coord3d,
	vcg::vertex::Normal3d,
	vcg::vertex::Mark,
	vcg::vertex::BitFlags,
	vcg::vertex::VFAdj,
	vcg::vertex::Qualityf> 
{
public:
	vcg::math::Quadric<double> &Qd() { return q; }
private:
	vcg::math::Quadric<double> q;
};

// Cette class est utilisée pour définir une face
class MCFace :
	public vcg::Face<MyUsedTypes,
	vcg::face::FFAdj,
	vcg::face::VertexRef,
	vcg::face::BitFlags,
	vcg::face::VFAdj,
	vcg::face::Normal3d,
	vcg::face::Mark,
	vcg::face::EFAdj>
{
public:
	int id = 0; // Identifiant de la face
};

// Cette class est utilisée pour définir une arête
class MCEdge :
	public vcg::Edge <MyUsedTypes,
	vcg::edge::EEAdj,
	vcg::edge::VertexRef,
	vcg::edge::BitFlags,
	vcg::edge::Mark,
	vcg::edge::Qualityf> {};

namespace mc {
	namespace mvcg {

		class MeshInfo;

		/*
		** Cette class est stock toutes les données d'un maillage
		*/
		class Mesh : public vcg::tri::TriMesh<std::vector<MCVertex>, std::vector<MCFace>, std::vector<MCEdge>>
		{
		public:
			Mesh() {};

			void TreeMake();

			mc::mvcg::MeshInfo *infos;

			vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info_buff;

			inline bool treeIsMake() { return this->_treeIsMake; };

			void decimate(int targetFaces, bool preserveBoundary);

			vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass> tree;

		private:
			bool _treeIsMake = false;
		};

		/*
		** Cette class permet de stocker quelques informations sur un maillage
		*/
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
};

#endif // !MC_VCG_MESH_H