#ifndef MC_VCG_OBJ_IMPORTER_H
#define MC_VCG_OBJ_IMPORTER_H

#include "mc_vcg_mesh.hpp"

#include <ctime>

namespace mc::mvcg
{

	int OpenObj(mc::mvcg::Mesh &mesh, const char *path);

	int SaveObj(mc::mvcg::Mesh &mesh, const char *path);

};

#endif //!MC_VCG_OBJ_IMPORTER_H