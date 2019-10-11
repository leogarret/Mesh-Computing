#include "mc_raycast.hpp"
using namespace mc::mvcg::ray;

Raycast::Raycast(const vec3 A, const vec3 B)
{
	originVec = A;
	directionVec = B;
}

