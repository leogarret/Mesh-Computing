#ifndef MC_VCG_OBJ_IMPORTER_H
#define MC_VCG_OBJ_IMPORTER_H

#include "mc_vcg_mesh.hpp"

#include <ctime>

namespace mc::mvcg
{

	int open_obj(mc::mvcg::Mesh &mesh, const char *path);

	int save_obj(mc::mvcg::Mesh &mesh, const char *path);

};

#endif //!MC_VCG_OBJ_IMPORTER_H