#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_obj_importer.hpp>
#include <mc_vcg_primitives.hpp>
#include <mc_vcg_intersection.hpp>

// VCG
#include <wrap\io_trimesh\export_obj.h>

#include <signal.h>

#include <io.h>

using namespace mc;

#undef max

#endif // !_MAIN_C

int main()
{
	LaunchDebugIntersection("../../obj/Mesh-20x20mm.obj");

	std::getchar();
	return 0;
}