#ifndef MC_TESTS_H
#define MC_TESTS_H

#include <iostream>

#include <mc_vcg_mesh.hpp>
#include <mc_vcg_primitives.hpp>
#include <mc_vcg_obj_importer.hpp>

#include <mc_om_mesh.hpp>
#include <mc_om_primitives.hpp>
#include <mc_om_obj_importer.hpp>

#include <mc_cm2_obj_importer.hpp>

namespace mc::tests {

	typedef const unsigned char TEST, LIB;

	/*
	---- TESTS ----
	-> CUBE_CREATE_TEST
	-> FIVE_MILLIONS_LOADING_TEST
	-> ALL_TEST
	---- LIBS ----
	-> VCG_LIB
	-> OPENMESH_LIB
	-> REMESH_LIB
	-> ALL_LIBS
	*/
	void launchTests(TEST _test = 0, LIB _lib = 0);

	enum TEST_NAME {
		CUBE_CREATE_TEST			= 0b0001,
		FIVE_MILLIONS_LOADING_TEST	= 0b0010,
		ALL_TEST					= 0b0011
	};

	enum LIB_NAME {
		VCG_LIB			= 0b0001,
		OPENMESH_LIB	= 0b0010,
		REMESH_LIB		= 0b0100,
		ALL_LIBS		= 0b0111
	};

};

#endif // !MC_TESTS_H
