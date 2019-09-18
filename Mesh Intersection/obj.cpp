#ifndef _OBJ_C
#define _OBJ_C

#include <iostream>
#include <ctime>
#include "obj.h"

#endif // !_OBJ_C

int obj::loader(Mesh &mesh, char *path, vcg::tri::io::ImporterOBJ<Mesh>::Info &mesh_info)
{
	OutputDebugString("OBJ en cours de chargement...\n");
	OutputDebugString(path); OutputDebugString("\n");
	int errorCode = vcg::tri::io::ImporterOBJ<Mesh>::Open(mesh, path, mesh_info);
	
	if (errorCode != 0 && errorCode != 5)
	{
		OutputDebugString("\nErreur lors du chargement de l'OBJ : ");
		OutputDebugString(vcg::tri::io::ImporterOBJ<Mesh>::ErrorMsg(errorCode));
		OutputDebugString("\n");
		return -1;
	}
	else if (errorCode == 5)
	{
		OutputDebugString(vcg::tri::io::ImporterOBJ<Mesh>::ErrorMsg(errorCode));
		OutputDebugString("\n");
	}

	OutputDebugString("OBJ charge avec succes.\n");

	return 1;
}

void obj::displayInfo(Mesh &mesh, vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info)
{
	std::cout << "******* MESH INFOS *******" << std::endl;
	std::cout << "Number of faces: " << mesh_info.numFaces << std::endl;
	std::cout << "Number of edges: " << mesh_info.numEdges << std::endl;
	std::cout << "Number of vertices: " << mesh_info.numVertices << std::endl;
	std::cout << "Surface: " << obj::surface::getPolygoneSurface(mesh) << std::endl;
}