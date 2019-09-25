#ifndef MC_VCG_OBJ_IMPORTER_H
#define MC_VCG_OBJ_IMPORTER_H

#include "mc_vcg_mesh.hpp"
#include <Windows.h>
#include <ctime>

namespace mc::mvcg::obj {

	int loader(mc::mvcg::Mesh &mesh, char *path);

};

#endif //!MC_VCG_OBJ_IMPORTER_H