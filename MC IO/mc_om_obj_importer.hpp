#pragma once
#ifndef MC_OM_OBJ_IMPORTER_H
#define MC_OM_OBJ_IMPORTER_H

#include <iostream>
#include <Windows.h>

#undef min
#undef max
#include <OpenMesh/Core/IO/MeshIO.hh>

#include "mc_om_mesh.h"

namespace mc::om::obj {

	bool loader(mc::om::Mesh &mesh, char *path);

}

#endif //!MC_OM_OBJ_IMPORTER_H

bool mc::om::obj::loader(mc::om::Mesh &mesh, char *path)
{
	OutputDebugString("OBJ en cours de chargement...\n");
	OutputDebugString(path); OutputDebugString("\n");

#ifdef __IN_TEST_MODE
	std::cout << "OBJ en cours de chargement...\n" << std::endl;
#endif // __IN_TEST_MODE

	std::clock_t start = 0, end = 0;
	try
	{
		start = std::clock();
		if (!OpenMesh::IO::read_mesh(mesh, path))
		{
			OutputDebugString("Cannot read file ");
			OutputDebugString(path); OutputDebugString("\n");
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
	_itoa_s(end - start, buff, 10);
	OutputDebugString("OBJ read in ");
	OutputDebugString((buff != NULL) ? buff : "?");
	OutputDebugString("ms.\n");

#ifdef __IN_TEST_MODE
	std::cout << "OBJ chargé avec succès en " << end - start << "ms." << std::endl;
#endif // __IN_TEST_MODE

	return true;
}