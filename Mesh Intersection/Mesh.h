#pragma once
#ifndef _MESH_H
#define _MESH_H

#include<vcg/complex/complex.h>
#include <wrap\io_trimesh\import_obj.h>

class MyVertex; class MyEdge; class MyFace;

struct MyUsedTypes : public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType,
	vcg::Use<MyEdge>::AsEdgeType,
	vcg::Use<MyFace>::AsFaceType> {};

class MyVertex : public vcg::Vertex<MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags> {};
class MyFace : public vcg::Face<MyUsedTypes, vcg::face::FFAdj, vcg::face::VertexRef, vcg::face::BitFlags> {};
class MyEdge : public vcg::Edge<MyUsedTypes> {};

class Mesh : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>, std::vector<MyEdge>> {};

#endif // !_MESH_H