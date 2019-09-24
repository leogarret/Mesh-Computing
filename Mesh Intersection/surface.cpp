//#ifndef _OBJ_SURFACE_C
//#define _OBJ_SURFACE_C
//
//#endif // !_OBJ_SURFACE_C
//
//long obj::surface::getPolygoneSurface(Mesh &mesh)
//{
//	if (mesh.IsEmpty()) return -1;
//
//	Mesh::VertexIterator vi = mesh.vert.begin();
//	std::vector<MyFace> vec;
//
//	for (auto elem : mesh.face)
//		vec.push_back(elem);
//
//	long surface = 0;
//	for (auto elem : vec)
//		surface += vcg::PolyArea(elem);
//
//	return surface;
//}