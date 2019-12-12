#ifndef MC_VCG_STL_IMPORTER_C
#define MC_VCG_STL_IMPORTER_C

#include "mc_vcg_stl_io.hpp"
#include "mc_utils.hpp"

#include <mtlogger\mtlogger.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#endif //!MC_VCG_STL_IMPORTER_C

/*
** Cette fonction permet d'importer un fichier au format STL.
*/
int mc::mvcg::open_stl(mc::mvcg::Mesh &mesh, const char *path)
{
	if (mc::utils::extensionCompare(path, "stl") == false)
	{
		mc::utils::trace("Le fichier n'est pas au format stl.");
		return -1;
	}

	std::clock_t start = std::clock();

	int load_mask = 0;
	int errorCode = vcg::tri::io::ImporterSTL<mc::mvcg::Mesh>::Open(mesh, path, load_mask);
	mesh.infos = new mc::mvcg::MeshInfo(mesh);
	mesh.infos->mask = load_mask;
	
	std::clock_t end = std::clock();

	if (errorCode != 0 && errorCode != 5)
	{
		mc::utils::trace("Erreur lors du chargement du STL [" + std::string(vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::ErrorMsg(errorCode)) + "]\n");
		return 0;
	}
	else if (errorCode == 5)
	{
		mc::utils::trace(std::string(vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::ErrorMsg(errorCode)) + "\n");
	}

	mc::utils::traceWithTime(std::string(path) + std::string(" chargé avec succés"), end - start);

	return 1;
}

int mc::mvcg::save_stl(mc::mvcg::Mesh& mesh, const char* path)
{
	std::clock_t start = std::clock();
	int errorCode = vcg::tri::io::ExporterSTL<mc::mvcg::Mesh>::Save(mesh, path, 0);

	if (errorCode != 0)
	{
		mc::utils::trace("Impossible d'exporter le maillage : " + std::string(vcg::tri::io::ExporterSTL<mc::mvcg::Mesh>::ErrorMsg(errorCode)) + "\n");
	}

	std::clock_t end = std::clock();

	mc::utils::traceWithTime(std::string(path) + std::string(" sauvegardé avec succés"), end - start);

	return 0;
}