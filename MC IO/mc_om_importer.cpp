#ifndef MC_OM_OBJ_IMPORTER_C
#define MC_OM_OBJ_IMPORTER_C

#include "mc_om_importer.hpp"

#endif //!MC_OM_OBJ_IMPORTER_C

bool mc::om::obj::loader(mc::om::Mesh &mesh, const char *path)
{
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

	return true;
}

bool mc::om::stl::loader(mc::om::Mesh &mesh, const char *path)
{
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
	OutputDebugString("STL read in ");
	OutputDebugString((buff != NULL) ? buff : "?");
	OutputDebugString("ms.\n");

	return true;
}