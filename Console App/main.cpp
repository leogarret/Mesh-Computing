#ifndef _MAIN_CPP_ENTRY
#define _MAIN_CPP_ENTRY

#include <iostream>

#include <mc_cgal_mesh.hpp>
#include <mc_cgal_obj_io.hpp>
#include <mc_cgal_intersection.hpp>

#include "CGAL/Polyhedron_incremental_builder_3.h"

#endif // !_MAIN_CPP_ENTRY

int main()
{

	mc::mcgal::Mesh m;
	mc::mcgal::cgOpenObj(m, "../../obj/cube.obj");

	//mc::mcgal::intersection::IntersectionData datas;

	//mc::mcgal::cgSaveObj(m, "OUTOBJ.obj");
	std::getchar();
	return 0;
}

/*mvcg::Mesh mesh;
mvcg::open_obj(mesh, "../../obj/Mesh-20x20mm.obj");

mvcg::intersection::IntersectionData datas;
vcg::Ray3d ray;
ray.SetOrigin(vcg::Point3d(-60, -60, -100));
ray.SetDirection(vcg::Point3d(0, 0, 100));

mvcg::intersection::Intersect(mesh, datas, ray);

printf("nb. = %i\n(%.2f, %.2f, %.2f)\n", datas.facesIntersections.size() , datas.getPointAt(0).X(), datas.getPointAt(0).Y(), datas.getPointAt(0).Z());

std::cout << datas.getObjectType(0) << std::endl;*/