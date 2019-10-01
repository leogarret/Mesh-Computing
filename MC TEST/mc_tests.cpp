#ifndef MY_TEST_CPP
#define MY_TEST_CPP

#include "mc_tests.h"
#include <iostream>

#include <logger.hpp>
using namespace logger;

#endif

void mc::tests::launchTests(TEST _test, LIB _lib)
{
	if (_test == 0 || _lib == 0)
	{
		mcLog(logStream, ERROR_LOG, "lauchTests doit recevoir au minimum 1 test et une bibliothèque.\n");
		return;
	}

	const char *fivem_obj_path = "../../obj/M5.obj";
	const char *fivem_ascii_stl_path = "../../obj/M5-ascii.stl";
	const char *fivem_bin_stl_path = "../../obj/M5-bin.stl";

	mcLog(logStream, "**************** COMPARAISON VCG, OPEN MESH, GTS, REMESH et CGAL ****************\n\n");
	mcLog(logStream, "Génération des tests...\n\n");

	std::clock_t start, end;
	
	/************************************************ TEST 1 ************************************************/
	if ((_test & CUBE_CREATE_TEST) == CUBE_CREATE_TEST)
	{
		mcLog(logStream, "*****************************\nTest n°1 : Création d'un cube OBJ\n*****************************\n");

		if ((_lib & VCG_LIB) == VCG_LIB)
		{
			mcLog(logStream, "\n*** VCG ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::mvcg::Mesh *mesh = NULL;
			try
			{
				mesh = new mc::mvcg::Mesh;
				mcLog(logStream, "Cube en cours de création...\n");
				start = std::clock();
				mc::mvcg::primitives::createCube(*mesh);
				end = std::clock();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
				exit(-1);
			}
			mcLog(logStream, "Cube créé avec succès en %ims.\n", end - start);
			delete mesh;
		}

		std::cout << std::endl;

		if ((_lib & OPENMESH_LIB) == OPENMESH_LIB)
		{
			mcLog(logStream, "\n*** OpenMesh ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::om::Mesh *mesh = NULL;
			try
			{
				mesh = new mc::om::Mesh;
				mcLog(logStream, "Cube en cours de création...\n");
				start = std::clock();
				mc::om::primitives::createCube(*mesh);
				end = std::clock();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
				exit(-1);
			}
			mcLog(logStream, "Cube créé avec succès en %ims.\n", end - start);
			delete mesh;
		}
	}

	/************************************************ TEST 2 ************************************************/
	if ((_test & M5_OBJ_LOADING_TEST) == M5_OBJ_LOADING_TEST)
	{
		mcLog(logStream, "\n*****************************\nTest n°2 : Import OBJ 5 MILLION\n*****************************\n");

		if ((_lib & VCG_LIB) == VCG_LIB)
		{
			mcLog(logStream, "\n*** VCG ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::mvcg::Mesh *mesh = NULL;
			try
			{
				mesh = new mc::mvcg::Mesh;
				mcLog(logStream, "OBJ en cours de chargement...\n");
				start = std::clock();
				mc::mvcg::obj::loader(*mesh, fivem_obj_path);
				end = std::clock();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
				exit(-1);
			}
			mcLog(logStream, "OBJ chargé avec succès en %ims.\n", end - start);
			delete mesh;
		}

		std::cout << std::endl;

		if ((_lib & OPENMESH_LIB) == OPENMESH_LIB)
		{
			mcLog(logStream, "\n*** OpenMesh ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::om::Mesh *mesh = NULL;
			try
			{
				mesh = new mc::om::Mesh;
				mcLog(logStream, "OBJ en cours de chargement...\n");
				start = std::clock();
				mc::om::obj::loader(*mesh, fivem_obj_path);
				end = std::clock();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
				exit(-1);
			}
			mcLog(logStream, "OBJ chargé avec succès en %ims.\n", end - start);
			delete mesh;
		}

		std::cout << std::endl;

		if ((_lib & REMESH_LIB) == REMESH_LIB)
		{
			cm2::surf_remesh_t3::registration("Licensed to POLYQUARK.", "9085405CBA53");
			mcLog(logStream, "*** ReMesh ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::mcm2::Mesh *mesh = NULL;
			try
			{
				mesh = new mc::mcm2::Mesh;
				mcLog(logStream, "OBJ en cours de chargement...\n");
				start = std::clock();
				mc::mcm2::obj::loader(*mesh, fivem_obj_path);
				end = std::clock();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
				exit(-1);
			}
			mcLog(logStream, "OBJ chargé avec succès en %ims.\n", end - start);
			delete mesh;
		}
	}

	/************************************************ TEST 3 ************************************************/
	if ((_test & M5ASCII_STL_LOADING_TEST) == M5ASCII_STL_LOADING_TEST)
	{
		mcLog(logStream, "\n*****************************\nTest n°3 : Import STL (ASCII) 5 MILLION\n*****************************\n");

		/*if ((_lib & VCG_LIB) == VCG_LIB)
		{
			mcLog(logStream, "\n*** VCG ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::mvcg::Mesh *mesh = NULL;
			try
			{
				mesh = new mc::mvcg::Mesh;
				mcLog(logStream, "STL en cours de chargement...\n");
				start = std::clock();
				mc::mvcg::stl::loader(*mesh, fivem_ascii_stl_path);
				end = std::clock();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
				exit(-1);
			}
			mcLog(logStream, "STL chargé avec succès en %ims.\n", end - start);
			delete mesh;
		}

		std::cout << std::endl;*/

		if ((_lib & OPENMESH_LIB) == OPENMESH_LIB)
		{
			mcLog(logStream, "\n*** OpenMesh ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::om::Mesh *mesh = NULL;
			try
			{
				mesh = new mc::om::Mesh;
				mcLog(logStream, "OBJ en cours de chargement...\n");
				start = std::clock();
				mc::om::obj::loader(*mesh, fivem_ascii_stl_path);
				end = std::clock();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caught: " << ba.what() << '\n';
				exit(-1);
			}
			mcLog(logStream, "OBJ chargé avec succès en %ims.\n", end - start);
			delete mesh;
		}

		/*std::cout << std::endl;

		if ((_lib & REMESH_LIB) == REMESH_LIB)
		{
			cm2::surf_remesh_t3::registration("Licensed to POLYQUARK.", "9085405CBA53");
			mcLog(logStream, "*** ReMesh ***\n");
			mcLog(logStream, "Initialisations...\n");
			mc::mcm2::Mesh *mesh = new mc::mcm2::Mesh();
			mcLog(logStream, "OBJ en cours de chargement...\n");
			start = std::clock();
			mc::mcm2::obj::loader(*mesh, "../../obj/M5.obj");
			end = std::clock();
			mcLog(logStream, "OBJ chargé avec succès en %ims.\n", end - start);
			delete mesh;
		}*/
	}

}