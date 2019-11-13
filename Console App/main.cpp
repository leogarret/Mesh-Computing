#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>
#include <mc_vcg_intersection.hpp>

// VCG
#include <wrap\io_trimesh\export_obj.h>

#include <signal.h>

using namespace mc;

//void debugIntersection();
OBJTYPE onObjectType(MCFace face, vcg::Point3d points);

#undef max

#endif // !_MAIN_C

int main()
{
	mc::mvcg::Mesh m;
	mc::mvcg::obj::loader(m, "../../obj/Mesh-20x20mm.obj");
	m.TreeMake();

	int i = 0;
	for (auto elem : m.face)
		elem.id = i++;

	std::cout << i << std::endl;
	mc::BuffIntersect buff;
	vcg::Ray3<MIndex::ScalarType, true> ray;
	ray.SetOrigin(vcg::Point3d(0, 50, 10));
	ray.SetDirection(vcg::Point3d(0, 0, -10));

	mc::Intersect(m, buff, ray);

	mt::log(stdout, "Nombre d'intersection(s): %i\n", buff.facesIntersections.size());

	

	std::getchar();

	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "PLAN.obj", 0);
	return 0;
}

