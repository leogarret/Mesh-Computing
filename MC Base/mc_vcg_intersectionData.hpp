#ifndef _INTERSECTION_DATA_HPP
#define _INTERSECTION_DATA_HPP

#include <mc_vcg_mesh.hpp>
#include "mc_vcg_intersection.hpp"

#include <ctime>

#ifndef TreeType
#define TreeType vcg::AABBBinaryTreeIndex<MCFace, double, vcg::EmptyClass>
#endif // !TreeType

namespace mc::mvcg::intersection
{
	/*
	** Cette class permet de stocker toutes les données d'une droite d'intersection.
	*/
	class IntersectionData
	{
	public:
		inline MCFace getFaceAt(int index) { return *facesIntersections.at(index); }
		inline TreeType::CoordType getPointAt(int index) { return pointsIntersections.at(index); }
		inline int getObjectType(int index) { return objectTypes.at(index); }

		inline int getNumberIntersections() { return facesIntersections.size(); }

		std::vector<MCFace*> facesIntersections;
		std::vector<TreeType::CoordType> pointsIntersections;
		std::vector<int> objectTypes;
	};
}

#endif // !_INTERSECTION_DATA_HPP