#ifndef MV_VCG_SURFACE_C
#define MV_VCG_SURFACE_C

#include "mc_vcg_algorithms.hpp"

#include <vcg/space/segment3.h>
#include <vcg/space/point3.h>
#include <vcg/space/plane3.h>

#endif //!MV_VCG_SURFACE_C



double mc::mvcg::algorithms::surface(mc::mvcg::Mesh &mesh)
{
	if (mesh.IsEmpty()) return -1;

	double area = 0;
	for (auto elem : mesh.face)
	{
		area += vcg::Norm((elem.cP(1) - elem.cP(0)) ^ (elem.cP(2) - elem.cP(0))) / 2;
	}

	return area;
}