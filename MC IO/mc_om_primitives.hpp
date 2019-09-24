#pragma once
#ifndef MC_OM_PRIMITIVES_H
#define MC_OM_PRIMITIVES_H

#include "mc_om_obj_importer.hpp"

namespace mc::om::primitives {

	int createCube(mc::om::Mesh &mesh);

}

#endif //!MC_OM_PRIMITIVES_H

int mc::om::primitives::createCube(mc::om::Mesh &mesh)
{
	OutputDebugString("Cube en cours de creation...\n");

#ifdef __IN_TEST_MODE
	std::cout << "Cube en cours de création...\n" << std::endl;
#endif // __IN_TEST_MODE

	std::clock_t start = std::clock();
	Mesh::VertexHandle vertexHandle[8];

	// Listes des vertices
	vertexHandle[0] = mesh.add_vertex(Mesh::Point(-1, -1, 1));
	vertexHandle[1] = mesh.add_vertex(Mesh::Point(1, -1, 1));
	vertexHandle[2] = mesh.add_vertex(Mesh::Point(1, 1, 1));
	vertexHandle[3] = mesh.add_vertex(Mesh::Point(-1, 1, 1));
	vertexHandle[4] = mesh.add_vertex(Mesh::Point(-1, -1, -1));
	vertexHandle[5] = mesh.add_vertex(Mesh::Point(1, -1, -1));
	vertexHandle[6] = mesh.add_vertex(Mesh::Point(1, 1, -1));
	vertexHandle[7] = mesh.add_vertex(Mesh::Point(-1, 1, -1));

	std::vector<Mesh::VertexHandle> faceVertexHandles;

	// Face 1 
	{
		faceVertexHandles.clear();
		faceVertexHandles.push_back(vertexHandle[0]);
		faceVertexHandles.push_back(vertexHandle[1]);
		faceVertexHandles.push_back(vertexHandle[2]);
		faceVertexHandles.push_back(vertexHandle[3]);
		mesh.add_face(faceVertexHandles);
	}

	// Face 2
	{
		faceVertexHandles.clear();
		faceVertexHandles.push_back(vertexHandle[7]);
		faceVertexHandles.push_back(vertexHandle[6]);
		faceVertexHandles.push_back(vertexHandle[5]);
		faceVertexHandles.push_back(vertexHandle[4]);
		mesh.add_face(faceVertexHandles);
}

	// Face 3
	{
		faceVertexHandles.clear();
		faceVertexHandles.push_back(vertexHandle[1]);
		faceVertexHandles.push_back(vertexHandle[0]);
		faceVertexHandles.push_back(vertexHandle[4]);
		faceVertexHandles.push_back(vertexHandle[5]);
		mesh.add_face(faceVertexHandles);
	}

	// Face 4
	{
		faceVertexHandles.clear();
		faceVertexHandles.push_back(vertexHandle[2]);
		faceVertexHandles.push_back(vertexHandle[1]);
		faceVertexHandles.push_back(vertexHandle[5]);
		faceVertexHandles.push_back(vertexHandle[6]);
		mesh.add_face(faceVertexHandles);
	}

	// Face 5
	{
		faceVertexHandles.clear();
		faceVertexHandles.push_back(vertexHandle[3]);
		faceVertexHandles.push_back(vertexHandle[2]);
		faceVertexHandles.push_back(vertexHandle[6]);
		faceVertexHandles.push_back(vertexHandle[7]);
		mesh.add_face(faceVertexHandles);
	}

	// Face 6
	{
		faceVertexHandles.clear();
		faceVertexHandles.push_back(vertexHandle[0]);
		faceVertexHandles.push_back(vertexHandle[3]);
		faceVertexHandles.push_back(vertexHandle[7]);
		faceVertexHandles.push_back(vertexHandle[4]);
		mesh.add_face(faceVertexHandles);
	}

	std::clock_t end = std::clock();

#ifdef __IN_TEST_MODE
	std::cout << "OBJ chargé avec succès en " << end - start << "ms." << std::endl;
#endif // __IN_TEST_MODE

	return 1;
}