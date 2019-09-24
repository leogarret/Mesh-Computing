#ifndef _MAIN_CCX
#define _MAIN_CCX

#include "main.h"

#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>

#include <mc_om_obj_importer.hpp>
#include <mc_om_primitives.hpp>

#include <mc_cm2_obj_importer.hpp>

#endif // !_MAIN_CCX



void launch_tests()
{
	std::cout << "**************** COMPARAISON VCG, OPEN MESH, GTS, REMESH et CGAL ****************" << std::endl;
	std::cout << "\nGénération des tests..." << std::endl;

	// TEST 1
	{
		std::cout << "\n*****************************\nTest n°1 : Création d'un cube\n*****************************\n" << std::endl;

		{
			std::cout << "*** VCG ***\n" << std::endl;
			std::cout << "Initialisations...\n" << std::endl;
			mc::mvcg::Mesh mesh;
			mc::mvcg::primitives::createCube(mesh);
		}

		{
			std::cout << "*** OpenMesh ***\n" << std::endl;
			std::cout << "Initialisations...\n" << std::endl;
			mc::om::Mesh mesh;
			mc::om::primitives::createCube(mesh);
		}
	}

	// TEST 2
	/*{
	std::cout << "\n*****************************\nTest n°2 : Import OBJ\n*****************************\n" << std::endl;
	{
	std::cout << "*** VCG ***\n" << std::endl;
	std::cout << "Initialisations...\n" << std::endl;
	mc::mvcg::Mesh mesh;
	mc::mvcg::obj::loader(mesh, "../../obj/M5.obj");
	}

	std::cout << std::endl;

	{
	std::cout << "*** OpenMesh ***\n" << std::endl;
	std::cout << "Initialisations...\n" << std::endl;
	mc::om::Mesh mesh;
	mc::om::obj::loader(mesh, "../../obj/M5.obj");
	}

	std::cout << std::endl;

	{
	cm2::surf_remesh_t3::registration("Licensed to POLYQUARK.", "9085405CBA53");
	std::cout << "*** ReMesh ***\n" << std::endl;
	std::cout << "Initialisations...\n" << std::endl;
	mc::mcm2::Mesh mesh;
	mc::mcm2::obj::loader(mesh, "../../obj/M5.obj");
	}
	}*/
}

int main(int ac, char **av)
{
	launch_tests();

	return 0;
}