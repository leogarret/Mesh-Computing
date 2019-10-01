#ifndef MC_CM2_OBJ_IMPORTER_H
#define MC_CM2_OBJ_IMPORTER_H

#include <iostream>
#include <ctime>

// MESHTOOLS
#include <cm2/surf_remesh_t3.h>

#include "mc_cm2_mesh.hpp"

namespace mc::mcm2::obj {

	int loader(mc::mcm2::Mesh &mesh, const char *path);

}

#endif // !MC_CM2_OBJ_IMPORTER_H