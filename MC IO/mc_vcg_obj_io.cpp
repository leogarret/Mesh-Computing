#ifndef MC_VCG_OBJ_IMPORTER_C
#define MC_VCG_OBJ_IMPORTER_C

#include "mc_vcg_obj_io.hpp"
#include "mc_utils.hpp"
#include <mtlogger\mtlogger.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#endif //!MC_VCG_OBJ_IMPORTER_C

/*
** Cette fonction permet d'importer un maillage au format obj.
** Elle retourne 1 si tout c'est bien passé.
*/
int mc::mvcg::open_obj(mc::mvcg::Mesh &mesh, const char *path)
{
	if (mc::utils::extensionCompare(path, "obj") == false)
	{
		mc::utils::trace("Le fichier n'est pas au format obj.");
		return -1;
	}

	std::clock_t start = std::clock();
	int errorCode = vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::Open(mesh, path, mesh.mesh_info_buff);
	mesh.infos = new mc::mvcg::MeshInfo(mesh.mesh_info_buff);
	std::clock_t end = std::clock();

	if (errorCode != 0 && errorCode != 5)
	{
		mc::utils::trace("Erreur lors du chargement de l'OBJ [" + std::string(vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::ErrorMsg(errorCode)) + "]\n");
		return 0;
	}

	else if (errorCode == 5)
	{
		mc::utils::trace(std::string(vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::ErrorMsg(errorCode)) + "\n");
	}

	mc::utils::traceWithTime(std::string(path) + std::string(" chargé avec succés"), end - start);

	return 1;
}

/*
** Cette fonction permet d'exporter un maillage au format obj.
** Elle retourne 1 si tout c'est bien passé.
*/
int mc::mvcg::save_obj(mc::mvcg::Mesh & mesh, const char * path)
{
	std::clock_t start = std::clock();
	int errorCode = vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(mesh, path, 0);

	if (errorCode != 0)
	{
		mc::utils::trace("Impossible d'exporter le maillage : " + std::string(vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::ErrorMsg(errorCode)) + "\n");
	}

	std::clock_t end = std::clock();

	mc::utils::traceWithTime(std::string(path) + std::string(" sauvegardé avec succés"), end - start);

	return 0;
}
