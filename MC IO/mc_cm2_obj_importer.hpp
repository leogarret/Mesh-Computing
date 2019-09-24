#ifndef MC_CM2_OBJ_IMPORTER_H
#define MC_CM2_OBJ_IMPORTER_H

#include <iostream>
#include <ctime>

// MESHTOOLS
#include "cm2/meshtools.h"
#include "cm2/surf_remesh_t3.h" 
#include "cm2/surf_remesh_q4.h"

namespace mc::mcm2 {

	class Mesh {
	public:
		cm2::surf_remesh_t3::mesher remesher;
		cm2::surf_remesh_t3::mesher::data_type data;
		cm2::DoubleMat mat, tex;
		cm2::UIntMat connectIt, connectQ;
	};

	namespace obj {

		int loader(mc::mcm2::Mesh &mesh, char *path);

	}

}

#endif // !MC_CM2_OBJ_IMPORTER_H

inline static void display_hdl(void*, unsigned, const char* msg)
{
	std::cout << msg << std::endl;
}

int mc::mcm2::obj::loader(mc::mcm2::Mesh &mesh, char *path)
{
	OutputDebugString("OBJ en cours de chargement...\n");
	OutputDebugString(path); OutputDebugString("\n");

#ifdef __IN_TEST_MODE
	std::cout << "OBJ en cours de chargement...\n" << std::endl;
#endif // __IN_TEST_MODE

	std::clock_t start = std::clock();
	if (cm2::meshtools::WavefrontOBJ_input("../../obj/M5.obj", mesh.data.pos, mesh.mat, mesh.tex, mesh.connectIt, mesh.connectQ) != 0)
	{
		std::cout << "Cannot open this OBJ file." << std::endl;
	}
	std::clock_t end = std::clock();

	cm2::meshtools::medit_output("../../obj/cube.obj", mesh.data.pos, mesh.data.connectM, (cm2::element_type)6);

	char buff[10];
	_itoa_s(end - start, buff, 10);
	OutputDebugString("OBJ charge avec succes [");
	OutputDebugString((buff != NULL) ? buff : "?");
	OutputDebugString("ms]\n");

#ifdef __IN_TEST_MODE
	std::cout << "OBJ chargé avec succès en " << end - start << "ms." << std::endl;
#endif // __IN_TEST_MODE

	return 0;
}