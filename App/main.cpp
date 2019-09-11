#ifndef _MAIN_C
#define _MAIN_C

#include <iostream>
#include <obj.h>
#include <ui.h>

#include <gtk-2.0\gtk\gtk.h>

#endif // !_MAIN_C

int main()
{
	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	obj::loader(mesh, "../../obj/cube.obj", mesh_info);
	obj::displayInfo(mesh, mesh_info);

	std::getchar();
	
	return 0;
}