#ifndef MC_OM_MESH_H
#define MC_OM_MESH_H

#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>

namespace mc::om {

	typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

}

#endif // !MC_OM_MESH_H