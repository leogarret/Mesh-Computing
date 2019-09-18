#ifndef _VCG_H
#define _VCG_H

/**** VCG ****/
#include <obj.h>

namespace vc {

	bool open_mesh(Mesh &mesh, char *path);
	bool create_cube(Mesh &mesh);

}

#endif // !_VCG_H