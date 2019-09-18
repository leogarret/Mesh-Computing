#pragma once
#include "openMesh.h"
#include "main.h"

using namespace om;

bool om::open_mesh(Mesh &mesh, char *path)
{
	std::clock_t start = 0, end = 0;

	try
	{
		start = std::clock();
		if (!OpenMesh::IO::read_mesh(mesh, path))
		{
			OutputDebugString("Cannot read file ");
			OutputDebugString(path); NEXT_LINE_DEBUG;
			return false;
		}
	}
	catch (std::exception &e)
	{
		OutputDebugString(e.what());
		return false;
	}
	end = std::clock();

	char buff[10];
	_itoa(end - start, buff, 10);
	OutputDebugString("OBJ read in ");
	OutputDebugString((buff != NULL) ? buff : "?");
	OutputDebugString("ms.\n");

	return true;
}

bool om::create_cube(Mesh &mesh)
{
	Mesh::VertexHandle vertexHandle[8];

	vertexHandle[0] = mesh.add_vertex(Mesh::Point(-1, -1, 1));
	vertexHandle[1] = mesh.add_vertex(Mesh::Point(1, -1, 1));
	vertexHandle[2] = mesh.add_vertex(Mesh::Point(1, 1, 1));
	vertexHandle[3] = mesh.add_vertex(Mesh::Point(-1, 1, 1));
	vertexHandle[4] = mesh.add_vertex(Mesh::Point(-1, -1, -1));
	vertexHandle[5] = mesh.add_vertex(Mesh::Point(1, -1, -1));
	vertexHandle[6] = mesh.add_vertex(Mesh::Point(1, 1, -1));
	vertexHandle[7] = mesh.add_vertex(Mesh::Point(-1, 1, -1));

	std::vector<Mesh::VertexHandle> faceVertexHandles;

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[0]);
	faceVertexHandles.push_back(vertexHandle[1]);
	faceVertexHandles.push_back(vertexHandle[2]);
	faceVertexHandles.push_back(vertexHandle[3]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[7]);
	faceVertexHandles.push_back(vertexHandle[6]);
	faceVertexHandles.push_back(vertexHandle[5]);
	faceVertexHandles.push_back(vertexHandle[4]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[1]);
	faceVertexHandles.push_back(vertexHandle[0]);
	faceVertexHandles.push_back(vertexHandle[4]);
	faceVertexHandles.push_back(vertexHandle[5]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[2]);
	faceVertexHandles.push_back(vertexHandle[1]);
	faceVertexHandles.push_back(vertexHandle[5]);
	faceVertexHandles.push_back(vertexHandle[6]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[3]);
	faceVertexHandles.push_back(vertexHandle[2]);
	faceVertexHandles.push_back(vertexHandle[6]);
	faceVertexHandles.push_back(vertexHandle[7]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[0]);
	faceVertexHandles.push_back(vertexHandle[3]);
	faceVertexHandles.push_back(vertexHandle[7]);
	faceVertexHandles.push_back(vertexHandle[4]);
	mesh.add_face(faceVertexHandles);

	try
	{
		if (!OpenMesh::IO::write_mesh(mesh, "output.obj"))
		{
			OutputDebugString("Cannot write mesh to file 'output.obj'.\n");
			return false;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return false;
	}
	return true;
}