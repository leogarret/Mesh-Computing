#ifndef MC_VCG_PRIMITIVES_C
#define MC_VCG_PRIMITIVES_C

#include "mc_vcg_primitives.hpp"

using namespace logger;

#endif //!MC_VCG_PRIMITIVES_C

int mc::mvcg::primitives::createCube(mc::mvcg::Mesh &mesh)
{
	std::clock_t start = std::clock();
	Mesh::VertexIterator vertexIt = vcg::tri::Allocator<Mesh>::AddVertices(mesh, 8);
	Mesh::FaceIterator faceIt = vcg::tri::Allocator<Mesh>::AddFaces(mesh, 12);

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
	for (int i = 0; i <= 7; ++i, ++vertexIt) {
		vertexPtr[i] = &*vertexIt;
		vertexIt->P() = Mesh::CoordType(vertexContainer[i]);
	}

	// Face 1
	{
		faceIt->V(0) = vertexPtr[0];
		faceIt->V(1) = vertexPtr[1];
		faceIt->V(2) = vertexPtr[2];
		/**************************/
		++faceIt;
		faceIt->V(0) = vertexPtr[2];
		faceIt->V(1) = vertexPtr[3];
		faceIt->V(2) = vertexPtr[0];
	}

	// Face 2
	{
		++faceIt;
		faceIt->V(0) = vertexPtr[7];
		faceIt->V(1) = vertexPtr[6];
		faceIt->V(2) = vertexPtr[5];
		/**************************/
		++faceIt;
		faceIt->V(0) = vertexPtr[5];
		faceIt->V(1) = vertexPtr[4];
		faceIt->V(2) = vertexPtr[7];
	}

	// Face 3
	{
		++faceIt;
		faceIt->V(0) = vertexPtr[1];
		faceIt->V(1) = vertexPtr[0];
		faceIt->V(2) = vertexPtr[4];
		/**************************/
		++faceIt;
		faceIt->V(0) = vertexPtr[4];
		faceIt->V(1) = vertexPtr[5];
		faceIt->V(2) = vertexPtr[1];
	}

	// Face 4
	{
		++faceIt;
		faceIt->V(0) = vertexPtr[2];
		faceIt->V(1) = vertexPtr[1];
		faceIt->V(2) = vertexPtr[5];
		/**************************/
		++faceIt;
		faceIt->V(0) = vertexPtr[5];
		faceIt->V(1) = vertexPtr[6];
		faceIt->V(2) = vertexPtr[2];
	}

	// Face 5
	{
		++faceIt;
		faceIt->V(0) = vertexPtr[3];
		faceIt->V(1) = vertexPtr[2];
		faceIt->V(2) = vertexPtr[6];
		/**************************/
		++faceIt;
		faceIt->V(0) = vertexPtr[6];
		faceIt->V(1) = vertexPtr[7];
		faceIt->V(2) = vertexPtr[3];
	}

	// Face 6
	{
		++faceIt;
		faceIt->V(0) = vertexPtr[0];
		faceIt->V(1) = vertexPtr[3];
		faceIt->V(2) = vertexPtr[7];
		/**************************/
		++faceIt;
		faceIt->V(0) = vertexPtr[7];
		faceIt->V(1) = vertexPtr[4];
		faceIt->V(2) = vertexPtr[0];
	}

	std::clock_t end = std::clock();

	return 1;
}