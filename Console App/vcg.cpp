#pragma once
#include "main.h"
#include "vcg.h"

bool vc::open_mesh(Mesh &mesh, char *path)
{
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	std::clock_t start = 0, end = 0;

	start = std::clock();
	int errorCode = vcg::tri::io::ImporterOBJ<Mesh>::Open(mesh, path, mesh_info);

	if (errorCode != 0 && errorCode != 5)
	{
		OutputDebugString("Cannot read file "); OutputDebugString(path); NEXT_LINE_DEBUG;
		return false;
	}
	else
	{
		char buff[10];
		_itoa(end - start, buff, 10);
		OutputDebugString("OBJ read in ");
		OutputDebugString((buff != NULL) ? buff : "?");
		OutputDebugString("ms.\n");
	}

	return true;
}

bool vc::create_cube(Mesh &mesh)
{
	Mesh::VertexIterator vertexIt = vcg::tri::Allocator<Mesh>::AddVertices(mesh, 8);
	Mesh::FaceIterator faceIt = vcg::tri::Allocator<Mesh>::AddFaces(mesh, 6);

	vcg::Point3f vertexContainer[8];
	vertexContainer[0] = { -1, -1, 1 };
	vertexContainer[1] = { 1, -1, 1 };
	vertexContainer[2] = { 1, 1, 1 };
	vertexContainer[3] = { -1, 1, 1 };
	vertexContainer[4] = { -1, -1, -1 };
	vertexContainer[5] = { 1, -1, -1 };
	vertexContainer[6] = { 1, 1, -1 };
	vertexContainer[7] = { -1, 1, -1 };
	vertexContainer[8] = NULL;

	Mesh::VertexPointer vertexPtr[9];
	for (int i = 0; i <= 7 ; ++i, ++vertexIt) {
		vertexPtr[i] = &*vertexIt;
		vertexIt->P() = Mesh::CoordType(vertexContainer[i]);
	}

	// Face 1
	faceIt->V(0) = vertexPtr[0];
	faceIt->V(1) = vertexPtr[1];
	faceIt->V(2) = vertexPtr[2];
	//faceIt->V(3) = vertexPtr[3];

	// Face 2
	++faceIt;
	faceIt->V(0) = vertexPtr[7];
	faceIt->V(1) = vertexPtr[6];
	faceIt->V(2) = vertexPtr[5];
	//faceIt->V(3) = vertexPtr[4];

	// Face 3
	++faceIt;
	faceIt->V(0) = vertexPtr[1];
	faceIt->V(1) = vertexPtr[0];
	faceIt->V(2) = vertexPtr[4];
	//faceIt->V(3) = vertexPtr[5];

	// Face 4
	++faceIt;
	faceIt->V(0) = vertexPtr[2];
	faceIt->V(1) = vertexPtr[1];
	faceIt->V(2) = vertexPtr[5];
	//faceIt->V(3) = vertexPtr[6];

	// Face 5
	++faceIt;
	faceIt->V(0) = vertexPtr[3];
	faceIt->V(1) = vertexPtr[2];
	faceIt->V(2) = vertexPtr[6];
	//faceIt->V(3) = vertexPtr[7];

	// Face 6
	++faceIt;
	faceIt->V(0) = vertexPtr[0];
	faceIt->V(1) = vertexPtr[3];
	faceIt->V(2) = vertexPtr[7];
	//faceIt->V(3) = vertexPtr[4];

	return true;
}