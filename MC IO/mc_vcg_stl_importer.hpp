#ifndef MC_VCG_STL_IMPORTER_H
#define MC_VCG_STL_IMPORTER_H

#include "mc_vcg_mesh.hpp"
#include <ctime>

namespace mc::mvcg::stl {

	int loader(mc::mvcg::Mesh &mesh, const char *path);

};

#endif //!MC_VCG_STL_IMPORTER_H