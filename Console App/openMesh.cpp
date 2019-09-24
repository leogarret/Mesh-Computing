//#pragma once
//#include "openMesh.h"
//#include "main.h"
//
//using namespace om;
//
//bool om::export_mesh(Mesh const mesh, char *output)
//{
//	try
//	{
//		if (!OpenMesh::IO::write_mesh(mesh, output))
//		{
//			OutputDebugString("Cannot write mesh to file ");
//			OutputDebugString(output); OutputDebugString("\n");
//			return false;
//		}
//	}
//	catch (std::exception& x)
//	{
//		std::cerr << x.what() << std::endl;
//		return false;
//	}
//	return true;
//}