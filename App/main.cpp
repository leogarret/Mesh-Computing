#include <Mesh Intersection\obj.h>
#include <Mesh Intersection\Mesh.h>

int main()
{
	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	obj::loader(mesh, "../../obj/cube.obj", mesh_info);

	obj::displayInfo(mesh, mesh_info);

	std::getchar();
	return 0;
}