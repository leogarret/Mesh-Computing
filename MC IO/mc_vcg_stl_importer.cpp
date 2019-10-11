#ifndef MC_VCG_STL_IMPORTER_C
#define MC_VCG_STL_IMPORTER_C

#include "mc_vcg_stl_importer.hpp"
#include <logger.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

using namespace logger;

#endif //!MC_VCG_STL_IMPORTER_C

int mc::mvcg::stl::loader(mc::mvcg::Mesh &mesh, const char *path)
{
	std::clock_t start = std::clock();
	int load_mask = 0;
	int errorCode = vcg::tri::io::ImporterSTL<mc::mvcg::Mesh>::Open(mesh, path, load_mask);
	mesh.infos = new mc::mvcg::MeshInfo(mesh);
	mesh.infos->mask = load_mask;
	
	std::clock_t end = std::clock();
	if (errorCode != 0 && errorCode != 5)
	{
		OutputDebugString("\nErreur lors du chargement du STL : ");
		OutputDebugString(vcg::tri::io::ImporterSTL<mc::mvcg::Mesh>::ErrorMsg(errorCode));
		OutputDebugString("\n");
		return 0;
	}
	else if (errorCode == 5)
	{
		OutputDebugString(vcg::tri::io::ImporterSTL<mc::mvcg::Mesh>::ErrorMsg(errorCode));
		OutputDebugString("\n");
	}

	char buff[10];
	_itoa_s(end - start, buff, 10);
	OutputDebugString("STL charge avec succes [");
	OutputDebugString((buff != NULL) ? buff : "?");
	OutputDebugString("ms]\n");

	return 1;
}