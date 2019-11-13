#include "mc_vcg_intersection.hpp"
#include <ctime>

using namespace mc;

MIndex::CoordType mc::getPositionWithDistAndDir(MIndex::CoordType origin, MIndex::CoordType dir, MIndex::ScalarType dist)
{
	MIndex::CoordType intersectionPos;

	intersectionPos.X() = origin.X() + (dist * dir.X());
	intersectionPos.Y() = origin.Y() + (dist * dir.Y());
	intersectionPos.Z() = origin.Z() - dist;

	return intersectionPos;
}

int mc::Intersect(mc::mvcg::Mesh &m, BuffIntersect &it, vcg::Ray3<MIndex::ScalarType, false> ray)
{
	int nombreIntersections = 0;

	MIndex::ObjPtr isectFace;
	MIndex::ScalarType intersectionDist;
	vcg::RayTriangleIntersectionFunctor<> rayIntersector;
	const MIndex::ScalarType maxDist = std::numeric_limits<double>::max();


	std::vector<MCFace> faceBuff;

	int buff = -1;
	int cnt = 0;

	while ((isectFace = m.tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
	{
		if (isectFace->id == buff) break;
		nombreIntersections++;
		it.facesIntersections.push_back(*&isectFace);
		it.pointsIntersections.push_back(getPositionWithDistAndDir(it.origin, it.direction, intersectionDist));
		// Suppression temporaire de la face en intersection
		faceBuff.push_back(*isectFace);

		buff = isectFace->id;
		m.face.erase(m.face.begin() + (isectFace->id - cnt));
		cnt++;
	}

	/*for (MCFace elem : faceBuff)
		m.face.insert(m.face.begin() + elem.id, elem);*/

	it.nbIntersections = nombreIntersections;
	return nombreIntersections;
}