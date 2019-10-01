#ifndef MC_VCG_OBJ_IMPORTER_C
#define MC_VCG_OBJ_IMPORTER_C

#include "mc_vcg_obj_importer.hpp"
#include "mc_utils.hpp"
#include <logger.hpp>

#endif //!MC_VCG_OBJ_IMPORTER_C

int mc::mvcg::obj::loader(mc::mvcg::Mesh &mesh, const char *path)
{
	if (mc::utils::extensionCompare(path, "obj") == false)
	{
		logger::mcLog(logger::errorStream, "%s is not an obj file.\n", path);
		return -1;
	}

	std::clock_t start = std::clock();
	int errorCode = vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::Open(mesh, path, mesh.mesh_info_buff);
	mesh.infos = new mc::mvcg::MeshInfo(mesh.mesh_info_buff);
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

	return 1;
}