/*
**	FICHIER: vcg-intersection.cpp
**
**	DESCRIPTION: Ce fichier contient le code source pour permettre de récupérer des intersections avec une droite sur un maillage
*/


#include <mc_vcg_mesh.hpp>				// pour avoir accès à la structure de donnée d'un maillage
#include <mc_vcg_obj_io.hpp>			
#include <mc_vcg_intersection.hpp>
#include <mc_vcg_intersectionData.hpp>
#include <mc_all_intersection.hpp>

using namespace mc;

int main()
{
	mvcg::Mesh mesh;
	mvcg::OpenObj(mesh, "../../obj/Mesh-20x20mm.obj");

	mvcg::intersection::IntersectionData datas;
	vcg::Ray3d ray(vcg::Point3d(-60, -50, 50), vcg::Point3d(0, 0, -50));

	mvcg::intersection::Intersect(mesh, datas, ray);

	printf("%i\n", datas.getNumberIntersections());
	printf("%i\n", datas.getObjectType(0));

	std::getchar();
	return 0;
}