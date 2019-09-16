#ifndef _CONSOLE_APP_MAIN_C
#define _CONSOLE_APP_MAIN_C

#include <iostream>
#include <obj.h>
#include <ctime>

#define _CRT_SECURE_NO_WARNINGS

#endif //!_CONSOLE_APP_MAIN_C

int main(void)
{
	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	//std::string path;

	/*std::cout << "Obj to open : ";
	std::cin >> path;
	
	char *c_path = new char[path.length() + 1];

	strcpy_s(c_path, path.length(), path.c_str());

	for (int i = 0; i < path.length(); i++)
		std::cout << c_path[i];
	unsigned int t = obj::loader(mesh, "../../obj/Ballon-17218triangles.obj", mesh_info);*/

	Mesh m;

	Mesh::VertexIterator vi = vcg::tri::Allocator<Mesh>::AddVertices(m, 3);
	Mesh::FaceIterator fi = vcg::tri::Allocator<Mesh>::AddFaces(m, 1);
	Mesh::VertexPointer ivp[4];

	ivp[0] = &*vi; vi->P() = Mesh::CoordType(0.0, 0.0, 0.0); ++vi;
	ivp[1] = &*vi; vi->P() = Mesh::CoordType(1.0, 0.0, 0.0); ++vi;
	ivp[2] = &*vi; vi->P() = Mesh::CoordType(0.0, 1.0, 0.0); ++vi;
	fi->V(0) = ivp[0];

	fi->V(1) = ivp[1];
	fi->V(2) = ivp[2];

	//obj::displayInfo(m, mesh_info);

	std::cout << "Vextex number : " << m.VN() << std::endl;
	std::cout << "Face number : "  << m.FN() << std::endl;

	std::getchar();

	return 0;
}