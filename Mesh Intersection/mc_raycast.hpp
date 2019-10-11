#ifndef _RAYCAST_H
#define _RAYCAST_H

#include <vector>
#include <vcg\space\point3.h>

#include "mc_vec3.hpp"
using namespace mc;

using vcg::Point3d;

namespace mc::mvcg::ray {

	class Raycast
	{
	public:
		Raycast(const vec3 A, const vec3 B);
		inline vec3 getOrigin() { return originVec; }
		inline vec3 getDirection() { return directionVec; }
		//inline vec3 getRay(double d) { return originVec + d * directionVec; }

		vec3 originVec, directionVec;
	};

}
#endif //!_RAYCAST_H