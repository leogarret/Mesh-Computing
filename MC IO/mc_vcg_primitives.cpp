#ifndef MC_VCG_PRIMITIVES_C
#define MC_VCG_PRIMITIVES_C

#include "mc_vcg_primitives.hpp"

#endif //!MC_VCG_PRIMITIVES_C

/*
** Cette fonction permet de créer un cube
*/
int mc::mvcg::primitives::createCube(mc::mvcg::Mesh &mesh)
{
	Mesh::VertexIterator vertexIt = vcg::tri::Allocator<Mesh>::AddVertices(mesh, 8);
	Mesh::FaceIterator faceIt = vcg::tri::Allocator<Mesh>::AddFaces(mesh, 12);

	mesh.vert[0].P() = Mesh::CoordType({ -1, -1, 1 });
	mesh.vert[1].P() = Mesh::CoordType({ 1, -1, 1 });
	mesh.vert[2].P() = Mesh::CoordType({ 1, 1, 1 });
	mesh.vert[3].P() = Mesh::CoordType({ -1, 1, 1 });
	mesh.vert[4].P() = Mesh::CoordType({ -1, -1, -1 });
	mesh.vert[5].P() = Mesh::CoordType({ 1, -1, -1 });
	mesh.vert[6].P() = Mesh::CoordType({ 1, 1, -1 });
	mesh.vert[7].P() = Mesh::CoordType({ -1, 1, -1 });

	// Face 1
	{
		faceIt->V(0) = &mesh.vert[0];
		faceIt->V(1) = &mesh.vert[1];
		faceIt->V(2) = &mesh.vert[2];
		/**************************/
		++faceIt;
		faceIt->V(0) = &mesh.vert[2];
		faceIt->V(1) = &mesh.vert[3];
		faceIt->V(2) = &mesh.vert[0];
	}

	// Face 2
	{
		++faceIt;
		faceIt->V(0) = &mesh.vert[7];
		faceIt->V(1) = &mesh.vert[6];
		faceIt->V(2) = &mesh.vert[5];
		/**************************/
		++faceIt;
		faceIt->V(0) = &mesh.vert[5];
		faceIt->V(1) = &mesh.vert[4];
		faceIt->V(2) = &mesh.vert[7];
	}

	// Face 3
	{
		++faceIt;
		faceIt->V(0) = &mesh.vert[1];
		faceIt->V(1) = &mesh.vert[0];
		faceIt->V(2) = &mesh.vert[4];
		/**************************/
		++faceIt;
		faceIt->V(0) = &mesh.vert[4];
		faceIt->V(1) = &mesh.vert[5];
		faceIt->V(2) = &mesh.vert[1];
	}

	// Face 4
	{
		++faceIt;
		faceIt->V(0) = &mesh.vert[2];
		faceIt->V(1) = &mesh.vert[1];
		faceIt->V(2) = &mesh.vert[5];
		/**************************/
		++faceIt;
		faceIt->V(0) = &mesh.vert[5];
		faceIt->V(1) = &mesh.vert[6];
		faceIt->V(2) = &mesh.vert[2];
	}

	// Face 5
	{
		++faceIt;
		faceIt->V(0) = &mesh.vert[3];
		faceIt->V(1) = &mesh.vert[2];
		faceIt->V(2) = &mesh.vert[6];
		/**************************/
		++faceIt;
		faceIt->V(0) = &mesh.vert[6];
		faceIt->V(1) = &mesh.vert[7];
		faceIt->V(2) = &mesh.vert[3];
	}

	// Face 6
	{
		++faceIt;
		faceIt->V(0) = &mesh.vert[0];
		faceIt->V(1) = &mesh.vert[3];
		faceIt->V(2) = &mesh.vert[7];
		/**************************/
		++faceIt;
		faceIt->V(0) = &mesh.vert[7];
		faceIt->V(1) = &mesh.vert[4];
		faceIt->V(2) = &mesh.vert[0];
	}

	return 1;
}