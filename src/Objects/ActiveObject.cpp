/*
 * ActiveObject.cpp
 *
 *  Created on: 01/ago/2017
 *      Author: Davide
 */

#include <cmath>
#include <algorithm>
#include <iostream>

#include "ActiveObject.h"
#include "Tracks/Track.h"

#include "Interaction/Interaction.h"

using namespace std;

int ActiveObject::nObjects = 0;

ActiveObject::ActiveObject(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen):
ID(nObjects),
zposition (zheight),
xlen(xlen),
ylen(ylen),
zlen(zlen),
xdisplace(xdisplace),
ydisplace(ydisplace)
{
	nObjects++; 
}

ActiveObject::~ActiveObject()
{
}

void ActiveObject::setZposition(double z)
{
  zposition = z;
  return;
} 


Option<Intersections> ActiveObject::computeIntersectionPoints(const Track & track) const
{
	Intersections points;
	
	double x0,y0,z0,phi,theta;
	x0 = track.pos.x;
	y0 = track.pos.y;
	z0 = track.pos.z;
	phi = track.dir.phi;
	theta = track.dir.theta;
	
	double x1, x2, y1, y2, z1, z2;

	double x = xdisplace;
	double y = ydisplace;
	double z = zposition;

	double dx = xlen *.5;
	double dy = ylen *.5;
	double dz = zlen *.5;
	
	double xmin, xmax, ymin, ymax, zmin, zmax;
	
	double smin, smax;
	
	//z
	z1 = (z-z0-dz)/(cos(theta));
	z2 = (z-z0+dz)/(cos(theta));
	zmin = std::min(z1, z2);
	zmax = std::max(z1, z2);
	
	
	//x
	if (phi == M_PI_2 or phi == 3*M_PI_2 or theta == 0 or theta == M_PI)
	{
		if (x0 < x-dx or x0 > x+dx)
			return No<Intersections>();
		
		xmin = -std::numeric_limits<double>::max();
		xmax = std::numeric_limits<double>::max();
	}
	else
	{
		x1 = (x-x0-dx)/(cos(phi)*sin(theta));
		x2 = (x-x0+dx)/(cos(phi)*sin(theta));
		xmin = std::min(x1,x2);
		xmax = std::max(x1,x2);
	}

	//y
	if (phi == 0 or phi == M_PI or theta == 0 or theta == M_PI)
	{
		if (y0 < y-dy or y0 > y+dy)
			return No<Intersections>();
			
		ymin = -std::numeric_limits<double>::max();
		ymax = std::numeric_limits<double>::max();
	}
	else
	{
		y1 = (y-y0-dy)/(sin(phi)*sin(theta));
		y2 = (y-y0+dy)/(sin(phi)*sin(theta));
		ymin = std::min(y1,y2);
		ymax = std::max(y1,y2);
	}

	smin = max({xmin, ymin, zmin});
	smax = min({xmax, ymax, zmax});
	
	points.enter = smin;
	points.exit = smax;
	
	if (smin > smax)
	{
		return No<Intersections>();
	}
	
	return points;
}
	
