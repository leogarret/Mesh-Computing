#ifndef MC_OM_OBJ_IMPORTER_H
#define MC_OM_OBJ_IMPORTER_H

#include <iostream>
#include <Windows.h>

#undef min
#undef max
#include <OpenMesh/Core/IO/MeshIO.hh>

#include "mc_om_mesh.hpp"

namespace mc::om::obj {

	bool loader(mc::om::Mesh &mesh, char *path);

};

#endif //!MC_OM_OBJ_IMPORTER_H