#ifndef MC_OM_PRIMITIVES_C
#define MC_OM_PRIMITIVES_C

#include "mc_om_primitives.hpp"

#endif //!MC_OM_PRIMITIVES_C

int mc::om::primitives::createCube(Mesh &mesh)
{
	OutputDebugString("Cube en cours de creation...\n");

#ifdef __IN_TEST_MODE
	std::cout << "Cube en cours de cr�ation...\n" << std::endl;
#endif // __IN_TEST_MODE

	std::clock_t start = std::clock();
	mc::om::Mesh::VertexHandle vertexHandle[8];

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
	std::cout << "OBJ charg� avec succ�s en " << end - start << "ms." << std::endl;
#endif // __IN_TEST_MODE

	return 1;
}