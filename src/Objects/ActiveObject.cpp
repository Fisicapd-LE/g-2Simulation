/*
 * ActiveObject.cpp
 *
 *  Created on: 01/ago/2017
 *      Author: Davide
 */

#include <cmath>
#include <algorithm>

#include "ActiveObject.h"
#include "Tracks/Track.h"

int ActiveObject::nObjects = 0;

ActiveObject::ActiveObject(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen):
zposition (zheight),
xdisplace(xdisplace),
ydisplace(ydisplace),
xlen(xlen),
ylen(ylen),
zlen(zlen),
ID(nObjects)
{
	nObjects++; 
}

ActiveObject::~ActiveObject()
{
}

void ActiveObject::SetZposition(double z)
{
  zposition = z;
  return;
} 


std::pair<double, double> ActiveObject::intersectionPoints(const Track & ray, double zmax) const
{
  auto points = ray.getIntersectionPoint(ID);
  if (std::isnan(points.first))
  {
	  double x0,y0,z0,phi,theta;
	  x0 = ray.pos.x;
	  y0 = ray.pos.y;
	  z0 = ray.pos.z;
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
	  
	  //x
	  if (phi == M_PI_2 or phi == 3*M_PI_2)
	  {
		if (x0 > x-dx and x0 < x+dx)
		{
		  min[0] = -std::numeric_limits<double>::max();
		  max[0] = std::numeric_limits<double>::max();
		}
		else
		  return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
	  }
	  else
	  {
		x1 = (x-x0-dx)/(cos(phi)*sin(theta));
		x2 = (x-x0+dx)/(cos(phi)*sin(theta));
		min[0] = std::max(0.,std::min(x1,x2));
		max[0] = std::max(0.,std::max(x1,x2));
	  }
	  
	  //y
	  if (phi == 0 or phi == M_PI)
	  {
		if (y0 > y-dy and y0 < y+dy)
		{
		  min[1] = -std::numeric_limits<double>::max();
		  max[1] = std::numeric_limits<double>::max();
		}
		else
		{
		  return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
		}
	  }
	  else
	  {
		y1 = (y-y0-dy)/(sin(phi)*sin(theta));
		y2 = (y-y0+dy)/(sin(phi)*sin(theta));
		min[1] = std::max(0.,std::min(y1,y2));
		max[1] = std::max(0.,std::max(y1,y2));
	  }
	  
	  //z
	  z1 = (z-z0-dz)/(cos(theta));
	  z2 = (z-z0+dz)/(cos(theta));
	  min[2] = std::max(0.,std::min(z1,z2));
	  max[2] = std::max(0.,std::max(z1,z2));
	  
	  points.first = min[0];
	  points.second = max[0];
	  for(int i = 0 ; i < 3; i++)
	  {
		if (points.first < min[i])
		  points.first = min[i];
		if (points.second > max[i])
		  points.second = max[i];
	  }
	  if (!std::isnan(zmax))
	  {
		  double sEnd = zmax/cos(theta);
		  points.second = std::min(points.second, sEnd);
	  }
	  ray.addIntersectionPoint(ID, points);
  }
  
  return points;
}


double ActiveObject::SpaceTravelled (const Track & ray, double zmax) const
{
  auto points = intersectionPoints(ray, zmax);
  double& Max = points.second;
  double& Min = points.first;
  
  if (not(Max > Min))
    return 0;
  
  return (Max - Min);
}
