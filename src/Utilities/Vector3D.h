#ifndef POSITION3D_H
#define POSITION3D_H

#include <cmath>

struct Vector3D
{
  double x;
  double y;
  double z;
};

using Position3D = Vector3D;
using B = Vector3D;

struct Direction
{
	double phi;
	double theta;
};

inline double prod(const Vector3D& lhs, const Vector3D& rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

inline double norm(const Vector3D& vec)
{
	return std::sqrt(prod(vec, vec));
}

inline Direction versor(const Vector3D& vec)
{
	return {std::atan2(vec.y, vec.x), std::acos(vec.z/norm(vec))};
}

inline Vector3D dirToVec(const Direction& dir, double mod)
{
	return {mod*sin(dir.phi)*sin(dir.theta), mod*cos(dir.phi)*sin(dir.theta), mod*cos(dir.theta)};
}

inline Vector3D cross(const Vector3D& lhs, const Vector3D& rhs)
{
	return {lhs.y*rhs.z-lhs.z*rhs.y, lhs.x*rhs.z-lhs.z*rhs.x, lhs.x*rhs.y-lhs.y*rhs.x};
}

#endif /* POSITION3D */
