#ifndef MC_VCG_OBJ_IMPORTER_H
#define MC_VCG_OBJ_IMPORTER_H

#include "mc_vcg_mesh.h"
#include <Windows.h>
#include <ctime>

namespace mc::mvcg::obj {

	int loader(mc::mvcg::Mesh &mesh, char *path);

}

#endif //!MC_VCG_OBJ_IMPORTER_H

int mc::mvcg::obj::loader(mc::mvcg::Mesh &mesh, char *path)
{
	OutputDebugString("OBJ en cours de chargement...\n");
	OutputDebugString(path); OutputDebugString("\n");

#ifdef __IN_TEST_MODE
	std::cout << "OBJ en cours de chargement...\n" << std::endl;
#endif // __IN_TEST_MODE

	std::clock_t start = std::clock();
	int errorCode = vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::Open(mesh, path, mesh.info);
	std::clock_t end = std::clock();
	if (errorCode != 0 && errorCode != 5)
	{
		OutputDebugString("\nErreur lors du chargement de l'OBJ : ");
		OutputDebugString(vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::ErrorMsg(errorCode));
		OutputDebugString("\n");
		return 0;
	}
	else if (errorCode == 5)
	{
		OutputDebugString(vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::ErrorMsg(errorCode));
		OutputDebugString("\n");
	}

	char buff[10];
	_itoa_s(end - start, buff, 10);
	OutputDebugString("OBJ charge avec succes [");
	OutputDebugString((buff != NULL) ? buff : "?");
	OutputDebugString("ms]\n");

#ifdef __IN_TEST_MODE
	std::cout << "OBJ charg� avec succ�s en " << end - start << "ms." << std::endl;
#endif // __IN_TEST_MODE

	return 1;
}