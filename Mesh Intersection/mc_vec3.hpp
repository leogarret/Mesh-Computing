#ifndef VEC3_HPP
#define VEC3_HPP

#include <math.h>
#include <stdlib.h>
#include <iostream>

namespace mc {

	class vec3 {

	public:
		vec3() {}; // Constructeur par défaut
		vec3(double x, double y, double z) { pos[0] = x; pos[1] = y; pos[2] = z; };

		inline double X() { return pos[0]; };
		inline double Y() { return pos[1]; };
		inline double Z() { return pos[2]; };

		inline const vec3& operator+() { return *this; };
		inline vec3 operator-() { return vec3(-pos[0], -pos[1], -pos[2]); }
		inline double operator[](int i) { return pos[i]; }

		vec3& operator+=(const vec3 &other);
		vec3& operator-=(const vec3 &other);
		vec3& operator*=(const vec3 &other);
		vec3& operator/=(const vec3 &other);
		vec3& operator*=(const double t);
		vec3& operator/=(const double t);

		float lenght();
		float squaredLenght();

		double pos[3];
	};

};

#endif