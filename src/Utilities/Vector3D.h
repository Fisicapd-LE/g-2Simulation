#ifndef POSITION3D_H
#define POSITION3D_H

#include <cmath>

struct Vector3D
{
  double x;
  double y;
  double z;
  double mod(){return std::sqrt(x*x+y*y+z*z);};
};

using Position3D = Vector3D;
using B = Vector3D;

#endif /* POSITION3D */
