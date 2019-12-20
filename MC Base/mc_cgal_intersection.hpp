#ifndef MC_CGAL_INTERSECTION_HPP
#define MC_CGAL_INTERSECTION_HPP

#include "mc_cgal_intersectionData.hpp"

typedef K::Ray_3																Ray;
typedef CGAL::Polyhedron_3<K>													Polyhedron;
typedef CGAL::AABB_face_graph_triangle_primitive<Polyhedron>					Primitive;
typedef CGAL::AABB_traits<K, Primitive>											Traits;
typedef CGAL::AABB_tree<Traits>													Tree;
typedef K::Segment_3															Segment;
typedef boost::optional<Tree::Intersection_and_primitive_id<Segment>::Type>		Segment_intersection;

namespace mc::mcgal::intersection
{
	typedef boost::optional<Tree::Intersection_and_primitive_id<Segment>::Type> Segment_intersection;

	void getInfoIntersection(mc::mcgal::intersection::IntersectionData& it, Segment_intersection intersection)
	{
		if (intersection)
		{
			auto buffP = boost::get<Point>(&(intersection->first));
			if (buffP)
			{
				it.points.push_back(buffP);
				it.objectTypes.push_back(T_POINT);
			}
		}
	}

	/*
	** Cette fonction permet de récuperer toutes les intersections d'une droite sur un maillage
	*/
	int Intersect(Mesh& mesh, IntersectionData& it, Ray& ray)
	{
		Polyhedron pol(mesh);
		Tree tree(faces(pol).first, faces(pol).second, pol);

		std::vector<Segment_intersection> intersections;
		tree.all_intersections(ray, std::back_inserter(intersections));

		for (auto elem : intersections)
		{
			getInfoIntersection(it, elem);
		}

		return 0;
	}

}

#endif //!MC_CGAL_INTERSECTION_HPP