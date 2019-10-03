#ifndef _MAIN_C
#define _MAIN_C

// MC (Mesh Computing)
#include <mc_vcg_algorithms.hpp>
#include <mc_vcg_edge.hpp>
#include <mc_vcg_primitives.hpp>


// VCG
#include <vcg/complex/algorithms/hole.h>
#include <wrap\io_trimesh\export_obj.h>


// LOGGER
#include <logger.hpp>
using namespace logger;


#endif // !_MAIN_C

int main(int ac, char **av)
{
	mc::mvcg::Mesh m1;
	mc::mvcg::primitives::createCube(m1);
	
	MyFace *f0 = &m1.face[0];
	MyFace *f1 = &m1.face[1];

	vcg::tri::UpdateTopology<mc::mvcg::Mesh>::FaceFace(m1);

	if (f0->FFp(2) == f1)
	{
		std::cout << "F0 and F1 share the same edge.\n" << std::endl;
	}
	else
	{
		mc::mvcg::edge::printFaceEdgeInfos(*f0);
		std::cout << std::endl;
		mc::mvcg::edge::printFaceEdgeInfos(*f1);
	}


	std::getchar();
	return 0;
}