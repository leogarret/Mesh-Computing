#ifndef _VCG_H
#define _VCG_H

/**** VCG ****/
#include <obj.h>

namespace vc {

	typedef struct MyData {
		int val1;
		int val2;
	} MYDATA, *PMYDATA;

	void start_vcg(void);
	bool open_mesh(Mesh &mesh, char *path);
	bool create_cube(Mesh &mesh);

}

#endif // !_VCG_H