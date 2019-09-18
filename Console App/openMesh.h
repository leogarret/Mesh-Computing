#pragma once
#ifndef _OPENMESH_H
#define _OPENMESH_H

#include <iostream>
#include <Windows.h>

/**** OPEN MESH ****/
#undef min
#undef max
#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

namespace om {

	typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

	bool open_mesh(Mesh &mesh, char *path);
	bool create_cube(Mesh &mesh);
	bool export_mesh(Mesh const mesh, char *output);
}

#endif //!_OPENMESH_H