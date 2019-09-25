#ifndef MC_VCG_OBJ_IMPORTER_C
#define MC_VCG_OBJ_IMPORTER_C

#include "mc_vcg_obj_importer.hpp"

#endif //!MC_VCG_OBJ_IMPORTER_C

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
	std::cout << "OBJ chargé avec succès en " << end - start << "ms." << std::endl;
#endif // __IN_TEST_MODE

	return 1;
}