#include "mc_vcg_intersection.hpp"
#include <ctime>


/*
** Cette fonction retourne une position calculée en fonction d'une origine, d'une direction et d'une distance
*/
MIndex::CoordType getPositionWithDistAndDir(MIndex::CoordType origin, MIndex::CoordType dir, MIndex::ScalarType dist)
{
	MIndex::CoordType intersectionPos;

	intersectionPos.X() = origin.X() + (dist * dir.X());
	intersectionPos.Y() = origin.Y() + (dist * dir.Y());
	intersectionPos.Z() = origin.Z() - dist;

	return intersectionPos;
}

bool faceCompare(MCFace a, MCFace b)
{
	int cnt = 0;

	a.P(0) == b.P(0) ? ++cnt : 0;
	a.P(1) == b.P(1) ? ++cnt : 0;
	a.P(2) == b.P(2) ? ++cnt : 0;

	if (cnt == 3)
	{
		std::cout << "OK!" << std::endl;
		/*printf("(%f, %f, %f) - (%f, %f, %f) - (%f, %f, %f)\n", a.P(0).X(), a.P(0).Y(), a.P(0).Z(), a.P(1).X(), a.P(1).Y(), a.P(1).Z(), a.P(2).X(), a.P(2).Y(), a.P(2).Z());
		printf("(%f, %f, %f) - (%f, %f, %f) - (%f, %f, %f)\n", b.P(0).X(), b.P(0).Y(), b.P(0).Z(), b.P(1).X(), b.P(1).Y(), b.P(1).Z(), b.P(2).X(), b.P(2).Y(), b.P(2).Z());*/
		return true;
	}
	return false;
}

int Intersect(std::vector<MCFace> mf, MIndex &tree, std::vector<MCFace*> &faces, std::vector<MIndex::CoordType> &points, vcg::Point3d origin, vcg::Point3d direction)
{
	// Déclaration des variables
	vcg::RayTriangleIntersectionFunctor<true> rayIntersector;
	const MIndex::ScalarType maxDist = std::numeric_limits<double>::max();
	MIndex::ScalarType intersectionDist = 0;
	MIndex::ObjPtr isectFace;
	MIndex::CoordType isectPt;

	// Création de la ligne d'intersection (Ray)
	const MIndex::CoordType rayOrigin(origin);
	MIndex::CoordType rayDirection(direction);
	vcg::Ray3<MIndex::ScalarType, true> ray(rayOrigin, rayDirection);

	int intersectionCounter = 0;
	while ((isectFace = tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
	{
		isectPt = getPositionWithDistAndDir(rayOrigin, rayDirection, intersectionDist);
		faces.push_back(isectFace);
		points.push_back(isectPt);
		//printf("Intersection[%i] with ray (%f, %f, %f)\n", intersectionCounter, ray.Origin().X(), ray.Origin().Y(), ray.Origin().Z());
		intersectionCounter++;

		/*int fit = 0;
		for (; faceCompare(mf[fit], *isectFace) == false && fit < mf.size(); ++fit);
		std::cout << fit << std::endl;
		mf.erase(mf.begin() + fit);*/


		// On supprime la face en intersection (pas la référence)
		int idx = 0;
		for (; idx < mf.size(); ++idx)
		{
			if (mf[idx].P(0) == isectFace->P(0))
			{
				if (mf[idx].P(1) == isectFace->P(1) &&
					mf[idx].P(2) == isectFace->P(2))
				{
					std::cout << idx << std::endl;
					mf.erase(mf.begin() + idx); // Suppression
				}
			}
		}
	}

	return intersectionCounter;
}