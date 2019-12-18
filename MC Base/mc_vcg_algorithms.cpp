#ifndef MV_VCG_SURFACE_C
#define MV_VCG_SURFACE_C

#include "mc_vcg_algorithms.hpp"

#include <vcg/space/segment3.h>
#include <vcg/space/point3.h>
#include <vcg/space/plane3.h>

#endif //!MV_VCG_SURFACE_C

/*
** Cette fonction permet de calculer l'aire d'un maillage
*/
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

/*
** Cette fonction permet de calculer la longueur totale des arêtes d'un maillage
*/
double mc::mvcg::algorithms::totalEdgesLenght(mc::mvcg::Mesh &mesh)
{
	if (mesh.IsEmpty()) return -1;

	double totalLenght = 0;
	for (auto elem : mesh.edge)
	{
		totalLenght += 1;
	}

	return totalLenght;
}

/*
** Cette fonction permet d'obtenir le barycentre d'un maillage
*/
vcg::Point3d mc::mvcg::algorithms::barycenter(mc::mvcg::Mesh &mesh)
{
	if (mesh.IsEmpty()) return NULL;

	vcg::Point3d baryCenter(0, 0, 0);
	int nb = 0;
	for (auto elem : mesh.vert)
	{
		baryCenter += elem.cP();
		nb++;
	}

	baryCenter[0] /= nb;
	baryCenter[1] /= nb;
	baryCenter[2] /= nb;

	return baryCenter;
}