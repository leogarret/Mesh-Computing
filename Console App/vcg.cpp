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