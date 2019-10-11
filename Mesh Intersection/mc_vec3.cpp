#include "mc_vec3.hpp"

using namespace mc;

mc::vec3 & mc::vec3::operator+=(const vec3 & other)
{
	pos[0] += other.pos[0];
	pos[1] += other.pos[1];
	pos[2] += other.pos[2];
	return *this;
}

mc::vec3 & mc::vec3::operator-=(const vec3 & other)
{
	pos[0] -= other.pos[0];
	pos[1] -= other.pos[1];
	pos[2] -= other.pos[2];
	return *this;
}

mc::vec3 & mc::vec3::operator*=(const vec3 & other)
{
	pos[0] *= other.pos[0];
	pos[1] *= other.pos[1];
	pos[2] *= other.pos[2];
	return *this;
}

mc::vec3 & mc::vec3::operator/=(const vec3 & other)
{
	pos[0] /= other.pos[0];
	pos[1] /= other.pos[1];
	pos[2] /= other.pos[2];
	return *this;
}

mc::vec3 & mc::vec3::operator*=(const double t)
{
	pos[0] *= t;
	pos[1] *= t;
	pos[2] *= t;
	return *this;
}

mc::vec3 & mc::vec3::operator/=(const double t)
{
	double k = 1.0 / t;
	pos[0] *= k;
	pos[1] *= k;
	pos[2] *= k;
	return *this;
}

inline mc::vec3 operator+(const vec3& v1, const vec3& v2)
{
	return vec3(v1.pos[0] + v2.pos[0], v1.pos[1] + v2.pos[1], v1.pos[2] + v2.pos[2]);
}

inline mc::vec3 operator-(const vec3& v1, const vec3& v2)
{
	return vec3(v1.pos[0] - v2.pos[0], v1.pos[1] - v2.pos[1], v1.pos[2] - v2.pos[2]);
}

inline mc::vec3 operator*(const vec3& v1, const vec3& v2)
{
	return vec3(v1.pos[0] * v2.pos[0], v1.pos[1] * v2.pos[1], v1.pos[2] * v2.pos[2]);
}

inline mc::vec3 operator/(const vec3& v1, const vec3& v2)
{
	return vec3(v1.pos[0] / v2.pos[0], v1.pos[1] / v2.pos[1], v1.pos[2] / v2.pos[2]);
}

inline mc::vec3 operator*(double d, const vec3& v)
{
	return vec3(d*v.pos[0], d*v.pos[1], d*v.pos[2]);
}

inline mc::vec3 operator*(const vec3& v, double d)
{
	return vec3(d*v.pos[0], d*v.pos[1], d*v.pos[2]);
}

inline mc::vec3 operator/(const vec3& v, double d)
{
	return vec3(v.pos[0] / d, v.pos[1] / d, v.pos[2] / d);
}

// OTHERS FUNCTIONS

float mc::vec3::lenght()
{
	return sqrt(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
}

float mc::vec3::squaredLenght()
{
	return pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2];
}