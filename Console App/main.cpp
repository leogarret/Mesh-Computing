#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>
#include <mc_vcg_intersection.hpp>

// VCG
#include <wrap\io_trimesh\export_obj.h>

#include <signal.h>

#include <io.h>

using namespace mc;

#undef max

#endif // !_MAIN_C

#define DI

int main()
{
#ifdef DI
	LaunchDebugIntersection("../../obj/Mesh-20x20mm.obj");
#elif defined(DT)

	/**********************************************************/

	mc::mvcg::Mesh mesh;
	mc::mvcg::obj::loader(mesh, "../../obj/Mesh-20x20mm.obj");

	int idIdx = 0;
	for (auto& elem : mesh.face)
		elem.id = idIdx++;

	mc::BuffIntersect buff;
	buff.origin = vcg::Point3d(-60.000000, -60.000000, 200.000000);
	buff.direction = vcg::Point3d(0.000000, 0.000000, -10.000000);

	vcg::Ray3d ray;
	ray.Set(buff.origin, buff.direction);

	int nb = mc::Intersect(mesh, buff, ray);

	std::cout << "Nb. intersections: " << nb << std::endl;
	std::cout << "Nb. faces: " << buff.facesIntersections.size() << std::endl;
#endif
	std::getchar();
	return 0;
}