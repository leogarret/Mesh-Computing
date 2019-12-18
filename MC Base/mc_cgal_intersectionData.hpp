#ifndef _CGAL_INTERSECTION_DATA_HPP
#define _CGAL_INTERSECTION_DATA_HPP

/* MY */
#include "mc_all_intersection.hpp"
#include "mc_cgal_mesh.hpp"

namespace mc::mcgal::intersection
{

	/*
	** Cette class permet de stocker toutes les données d'une droite d'intersection.
	*/
	class IntersectionData
	{
	public:
		inline Point getPointAt(int index) { return *points.at(index); }
		inline int getObjectType(int index) { return objectTypes.at(index); }

		inline int getNumberIntersections() { return points.size(); }
		
		std::vector<Point*> points;
		std::vector<int> objectTypes;
	};

};

//BuffIntersection getInfoIntersection(Segment_intersection intersection);

#endif // !_CGAL_INTERSECTION_DATA_HPP