#include "mc_vcg_intersection.hpp"
#include <ctime>

struct BuffIntersect {
	int nbIntersections;
	std::vector<MCFace*> facesIntersections;
	std::vector<MIndex::CoordType> pointsIntersections;
	vcg::Point3d origin;
	vcg::Point3d direction;
	std::clock_t time;
};

MIndex::CoordType getPositionWithDistAndDir(MIndex::CoordType origin, MIndex::CoordType dir, MIndex::ScalarType dist)
{
	MIndex::CoordType intersectionPos;

	intersectionPos.X() = origin.X() + (dist * dir.X());
	intersectionPos.Y() = origin.Y() + (dist * dir.Y());
	intersectionPos.Z() = origin.Z() - dist;

	return intersectionPos;
}

int Intersect(mc::mvcg::Mesh &m, BuffIntersect it)
{
	//int nombreIntersections = 0;
	//vcg::Ray3<MIndex::ScalarType, true> ray;

	//MIndex::ObjPtr isectFace;
	//MIndex::ScalarType intersectionDist;
	//vcg::RayTriangleIntersectionFunctor<> rayIntersector;
	//const MIndex::ScalarType maxDist = std::numeric_limits<double>::max();

	//std::vector<MCFace> faceBuff;

	//int buff = 0;
	//int cnt = 0;

	//while ((isectFace = m.tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
	//{
	//	if (isectFace->id == buff) break;

	//	nombreIntersections++;
	//	it.facesIntersections.push_back(*&isectFace);
	//	it.pointsIntersections.push_back(getPositionWithDistAndDir(it.origin, it.direction, intersectionDist));
	//	// Suppression temporaire de la face en intersection
	//	faceBuff.push_back(*isectFace);

	//	/*std::cout << "Intersection " << it.nbIntersections << std::endl;
	//	std::cout << m.face.at(isectFace->id).id << " - " << isectFace->id << std::endl;*/
	//	buff = isectFace->id;
	//	m.face.erase(m.face.begin() + (isectFace->id - cnt));
	//	cnt++;
	//	/*std::cout << m.face.at(isectFace->id - cnt).id << " - " << isectFace->id << std::endl;*/
	//}

	//return nombreIntersections;
}