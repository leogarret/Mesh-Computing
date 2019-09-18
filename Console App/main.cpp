#pragma once
#include "main.h"
#include "openMesh.h"
#include "vcg.h"

// VCG_MY if using VCG and OPEN_MESH_MY if using OpenMesh
#define VCG_MY

int main(void)
{
#ifdef OPENMESH_MY
	om::Mesh mesh;

	if (!om::open_mesh(mesh, "../../obj/M5.obj"))
		return -1;

	om::Mesh::VertexIter vertexIterator = mesh.vertices_begin();
	
	int i = 1;
	std::clock_t start = std::clock();
	for (; vertexIterator != mesh.vertices_end(); ++vertexIterator, ++i);
	std::clock_t end = std::clock();

	std::cout << i << "\n" << "Iterate in " << end - start << "ms" << std::endl;
#endif

#ifdef VCG_MY
	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;
	vc::create_cube(mesh);
	vcg::tri::io::ImporterOBJ<Mesh>::VertexIterator vertexIterator = mesh.vert.begin();


	vcg::tri::io::ExporterOBJ<Mesh>::Save(mesh, "cube_vcg.obj", NULL, NULL);
	/*int i = 1;
	std::clock_t start = std::clock();
	for (; vertexIterator != mesh.vert.end(); ++vertexIterator, ++i);
	std::clock_t end = std::clock();
	std::cout << i << "\n" << "Iterate in " << end - start << "ms" << std::endl;*/

#endif
	std::getchar();
	return 0;
}