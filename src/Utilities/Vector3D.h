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

#endif /* POSITION3D */
