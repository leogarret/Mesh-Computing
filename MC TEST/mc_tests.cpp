#ifndef MY_TEST_CPP
#define MY_TEST_CPP

#include "mc_tests.h"

#endif

void mc::tests::launchTests(TEST _test, LIB _lib)
{
	if (_test == 0 || _lib == 0)
	{
		std::cout << "ERREUR: lauchTests doit recevoir au minimum 1 test et une bibliothèque." << std::endl;
		return;
	}

	std::cout << "**************** COMPARAISON VCG, OPEN MESH, GTS, REMESH et CGAL ****************" << std::endl;
	std::cout << "\nGénération des tests..." << std::endl;

	// TEST 1
	if ((_test & CUBE_CREATE_TEST) == CUBE_CREATE_TEST)
	{
		std::cout << "\n*****************************\nTest n°1 : Création d'un cube\n*****************************\n" << std::endl;

		if ((_lib & VCG_LIB) == VCG_LIB)
		{
			std::cout << "*** VCG ***\n" << std::endl;
			std::cout << "Initialisations...\n" << std::endl;
			mc::mvcg::Mesh *mesh = new mc::mvcg::Mesh();
			mc::mvcg::primitives::createCube(*mesh);
			delete mesh;
		}

		std::cout << std::endl;

		if ((_lib & OPENMESH_LIB) == OPENMESH_LIB)
		{
			std::cout << "*** OpenMesh ***\n" << std::endl;
			std::cout << "Initialisations...\n" << std::endl;
			mc::om::Mesh *mesh = new mc::om::Mesh();
			mc::om::primitives::createCube(*mesh);
			delete mesh;
		}
	}

	// TEST 2
	if ((_test & FIVE_MILLIONS_LOADING_TEST) == FIVE_MILLIONS_LOADING_TEST)
	{
		std::cout << "\n*****************************\nTest n°2 : Import OBJ\n*****************************\n" << std::endl;

		if ((_lib & VCG_LIB) == VCG_LIB)
		{
			std::cout << "*** VCG ***\n" << std::endl;
			std::cout << "Initialisations...\n" << std::endl;
			mc::mvcg::Mesh *mesh = new mc::mvcg::Mesh();
			mc::mvcg::obj::loader(*mesh, "../../obj/M5.obj");
			delete mesh;
		}

		std::cout << std::endl;

		if ((_lib & OPENMESH_LIB) == OPENMESH_LIB)
		{
			std::cout << "*** OpenMesh ***\n" << std::endl;
			std::cout << "Initialisations...\n" << std::endl;
			mc::om::Mesh *mesh = new mc::om::Mesh();
			mc::om::obj::loader(*mesh, "../../obj/M5.obj");
			delete mesh;
		}

		std::cout << std::endl;

		if ((_lib & REMESH_LIB) == REMESH_LIB)
		{
			cm2::surf_remesh_t3::registration("Licensed to POLYQUARK.", "9085405CBA53");
			std::cout << "*** ReMesh ***\n" << std::endl;
			std::cout << "Initialisations...\n" << std::endl;
			mc::mcm2::Mesh *mesh = new mc::mcm2::Mesh();
			mc::mcm2::obj::loader(*mesh, "../../obj/M5.obj");
			delete mesh;
		}
	}
}