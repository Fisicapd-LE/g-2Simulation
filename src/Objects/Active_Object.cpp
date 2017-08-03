/*
 * Active_Object.cpp
 *
 *  Created on: 01/ago/2017
 *      Author: Davide
 */

#include <limits>
#include <cmath>

#include "Active_Object.h"
#include "Tracks/Track.h"



using namespace std;

Active_Object::Active_Object(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen):
zposition (zheight),
xdisplace(xdisplace),
ydisplace(ydisplace),
xlen(xlen),
ylen(ylen),
zlen(zlen),
Xentering_point(0.),
Yentering_point(0.),
Zentering_point(0.)
{ }

Active_Object::~Active_Object()
{ }

void Active_Object::SetZposition(double z)
{
  zposition = z;
  return;
} 


double Active_Object::SpaceTravelled (const Track & ray, Position3D * start) const
{
  double x0,y0,phi,theta;
  x0 = ray.pos.x;
  y0 = ray.pos.y;
  phi = ray.dir.phi;
  theta = ray.dir.theta;
  double min[3], max[3];
  double x1, x2, y1, y2, z1, z2;
  
  double x = xdisplace;
  double y = ydisplace;
  double z = zposition;
  
  double dx = xlen *.5;
  double dy = ylen *.5;
  double dz = zlen *.5;
  
//   double ratio = conv_ratio;
  
  //x
  if (phi == M_PI_2 or phi == 3*M_PI_2)
  {
    if (x0 > x-dx and x0 < x+dx)
    {
      min[0] = -numeric_limits<double>::max();
      max[0] = numeric_limits<double>::max();
    }
    else
      return 0;
  }
  else
  {
    x1 = (x-x0-dx)/(cos(phi)*sin(theta));
    x2 = (x-x0+dx)/(cos(phi)*sin(theta));
    if(x1 < x2)
    {
      min[0] = x1;
      max[0] = x2;
    }
    else
    {
      min[0] = x2;
      max[0] = x1;
    }
  }
  
  //y
  if (phi == 0 or phi == M_PI)
  {
    if (y0 > y-dy and y0 < y+dy)
    {
      min[1] = -numeric_limits<double>::max();
      max[1] = numeric_limits<double>::max();
    }
    else
    {
      return 0;
    }
  }
  else
  {
    y1 = (y-y0-dy)/(sin(phi)*sin(theta));
    y2 = (y-y0+dy)/(sin(phi)*sin(theta));
    if(y1 < y2)
    {
      min[1] = y1;
      max[1] = y2;
    }
    else
    {
      min[1] = y2;
      max[1] = y1;
    }
  }
  
  //z
  z1 = (z-dz)/(cos(theta));
  z2 = (z+dz)/(cos(theta));
  if(z1 < z2)
  {
    min[2] = z1;
    max[2] = z2;
  }
  else
  {
    min[2] = z2;
    max[2] = z1;
  }
  
  //Filling protected members of the class
//   Xentering_point = x1;
//   Yentering_point = y1;
//   Xentering_point = z1;

  entering_point.x = x1;
  entering_point.y = y1;
  entering_point.z = z1;
  
  exit_point.x = x2;
  exit_point.y = y2;
  exit_point.z = z2;
  
  //In case the track starts inside the active_object
  if(start != nullptr)
  {
    if(theta < M_PI/2) //track looks down
    {
      x1 = start->x;
      y1 =start->y;
      z1 = start->z;
    } else if(theta > M_PI/2) //track looks up
    {
      x2 = start->x;
      y2 =start->y;
      z2 = start->z;
    } else return 0; //not completely true, but we are not interested in tracks moving horizontally
  }
  
  double Min, Max;
  Min = min[0];
  Max = max[0];
  for(int i = 0 ; i < 3; i++)
  {
    //clog << "Min" << i << " " << min[i] << endl;
    //clog << "Max" << i << " " << max[i] << endl;
    if (Min < min[i])
      Min = min[i];
    if (Max > max[i])
      Max = max[i];
  }
  
  if (Max < Min)
    return 0;
  
  return (Max - Min);
}
