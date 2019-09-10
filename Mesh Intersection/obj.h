#ifndef _OBJ_H
#define _OBJ_H

#include "Mesh.h"

namespace obj {

	long loader(Mesh &mesh, char *path, vcg::tri::io::ImporterOBJ<Mesh>::Info &mesh_info);
	void displayInfo(Mesh &mesh, vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info);

	namespace surface {

		long getPolygoneSurface(Mesh &mesh);

	}

};

#endif //!_OBJ_H