#ifndef MC_CGAL_OBJ_IMPORTER_H
#define MC_CGAL_OBJ_IMPORTER_H

#include "mc_cgal_mesh.hpp"

namespace mc::mcgal
{

	void SaveObj(Mesh& mesh, const char* path);

};

#endif //!MC_CGAL_OBJ_IMPORTER_H