/*
**	FICHIER: cgal-intersection.cpp
**
**	DESCRIPTION: Ce fichier contient le code source pour permettre de récupérer des intersections avec une droite sur un maillage
*/


#include <mc_cgal_intersection.hpp> // Pour accéder aux fonctions d'intersections
#include <mc_cgal_intersectionData.hpp>
#include <mc_cgal_mesh.hpp>
#include <mc_cgal_obj_io.hpp>

int main()
{
	mc::mcgal::Mesh mesh; // Création du maillage
	mc::mcgal::OpenObj(mesh, "../../obj/Sphere-632586.obj"); // Chargement du maillage

	mc::mcgal::intersection::IntersectionData datas; // Ici seront stokées les données de la droite d'intersection
	Ray r(Point(0, 0, 50), Point(0, 0, -50)); // Paramètrage de la droite d'intersection

	mc::mcgal::intersection::Intersect(mesh, datas, r); // On récupère les intersections de la droite

	printf("Il y a eu %i intersections.\n", datas.getNumberIntersections()); // Affichage du nombre d'intersections

	// Ne pas hésiter à afficher les autres informations (membres de datas)

	std::getchar();
	return 0;
}