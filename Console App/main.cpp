#ifndef _MAIN_C
#define _MAIN_C

#include <mc_tests.h>
#include <mc_vcg_algorithms.hpp>
#include <wrap\io_trimesh\export_obj.h>

#include <logger.hpp>
using namespace logger;

#include <vcg/complex/algorithms/hole.h>

#include <ctime>

#endif // !_MAIN_C

int main(int ac, char **av)
{
	mc::mvcg::Mesh m1;

	mc::mvcg::obj::loader(m1, "../../obj/cube.obj");

	vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::VertexIterator vit = m1.vert.begin();
	vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::VertexIterator vit_end = m1.vert.end();
	for (; vit != vit_end; ++vit)
	{
		mcLog(logStream, "[%.2f][%.2f][%.2f]\n", vit[0].P()[0], vit[0].P()[1], vit[0].P()[2]);
	}

	std::getchar();

	return 0;
}