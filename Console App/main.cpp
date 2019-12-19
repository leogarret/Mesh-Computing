/*
**	FICHIER: cgal-intersection.cpp
**
**	DESCRIPTION: Ce fichier contient le code source pour permettre de r�cup�rer des intersections avec une droite sur un maillage
*/


#include <mc_cgal_intersection.hpp> // Pour acc�der aux fonctions d'intersections
#include <mc_cgal_intersectionData.hpp>
#include <mc_cgal_mesh.hpp>
#include <mc_cgal_obj_io.hpp>

int main()
{
	mc::mcgal::Mesh mesh; // Cr�ation du maillage
	mc::mcgal::OpenObj(mesh, "../../obj/Sphere-632586.obj"); // Chargement du maillage

	mc::mcgal::intersection::IntersectionData datas; // Ici seront stok�es les donn�es de la droite d'intersection
	Ray r(Point(0, 0, 50), Point(0, 0, -50)); // Param�trage de la droite d'intersection

	mc::mcgal::intersection::Intersect(mesh, datas, r); // On r�cup�re les intersections de la droite

	printf("Il y a eu %i intersections.\n", datas.getNumberIntersections()); // Affichage du nombre d'intersections

	// Ne pas h�siter � afficher les autres informations (membres de datas)

	std::getchar();
	return 0;
}