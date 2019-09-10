#ifndef _MAIN_C
#define _MAIN_C

#include <iostream>
#include <obj.h>

#endif // !_MAIN_C

int main(int argc, char **argv)
{
	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	std::cout << obj::loader(mesh, "../../obj/BARLAM_L.obj", mesh_info) << std::endl;

	//Mesh::PerVertexAttributeHandle<float> named_hv = vcg::tri::Allocator<Mesh>::GetPerVertexAttribute<float>(mesh, std::string("Irradiance"));
	

	obj::displayInfo(mesh, mesh_info);
	

	std::getchar();
	return 0;
}