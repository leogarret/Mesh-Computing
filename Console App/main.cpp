#ifndef _MAIN_CPP_ENTRY
#define _MAIN_CPP_ENTRY

// MC (Mesh Computing)
#include <mc_vcg_obj_io.hpp>
#include <mc_vcg_primitives.hpp>
#include <mc_vcg_intersection.hpp>

using namespace mc;

#endif // !_MAIN_CPP_ENTRY

int main()
{
	mvcg::Mesh mesh;
	mvcg::open_obj(mesh, "../../obj/Sphere-632586.obj");
	mvcg::intersection::IntersectionData datas;
	vcg::Ray3d ray;
	ray.Set(vcg::Point3d(-40, -20, 30), vcg::Point3d(0, 0, -30));

	mesh.TreeMake();

	mvcg::intersection::Intersect(mesh, datas, ray);
	
	for (auto elem : datas.pointsIntersections)
	{
		std::cout << elem.X() << " " << elem.Y() << " " << elem.Z() << std::endl;
	}

	mvcg::save_obj(mesh, "OUT.obj");

	std::getchar();
	return 0;
}