#ifndef MC_CGAL_INTERSECTION_HPP
#define MC_CGAL_INTERSECTION_HPP

#include "mc_cgal_intersectionData.hpp"

namespace mc::mcgal::intersection
{
	int cgIntersect(Mesh &mesh, IntersectionData &it, Ray &ray);

	//OBJTYPE onObjectType(MCFace face, vcg::Point3d points);

}

#endif //!MC_CGAL_INTERSECTION_HPP