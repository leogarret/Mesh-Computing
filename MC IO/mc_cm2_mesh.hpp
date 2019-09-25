#ifndef MC_CM2_MESH_H
#define MC_CM2_MESH_H

#include <cm2/meshtools.h>
#include <cm2/surf_remesh_t3.h> 

namespace mc::mcm2 {

	class Mesh {
	public:
		cm2::surf_remesh_t3::mesher remesher;
		cm2::surf_remesh_t3::mesher::data_type data;
		cm2::DoubleMat mat, tex;
		cm2::UIntMat connectIt, connectQ;
	};

}

#endif // !MC_CM2_MESH_H