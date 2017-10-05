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
zposition (zheight),
xlen(xlen),
ylen(ylen),
zlen(zlen),
xdisplace(xdisplace),
ydisplace(ydisplace),
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


Intersections ActiveObject::intersectionPoints(const Track & track, const Interaction& inter) const
{
	if(inter.completed())
		return inter.getIntersectionPoint(ID);
		
	if (inter.intersectionsComputed(ID))
		return inter.getIntersectionPoint(ID, false);
	
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
	if (zmin > track.getEnd() or zmax < track.getStart())
		return Intersections{No<double>(), No<double>()};
	
	
	//x
	if (phi == M_PI_2 or phi == 3*M_PI_2 or theta == 0 or theta == M_PI)
	{
		if (x0 < x-dx or x0 > x+dx)
			return Intersections{No<double>(), No<double>()};
		
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
	if (xmin > track.getEnd() or xmax < track.getStart())
		return Intersections{No<double>(), No<double>()};

	//y
	if (phi == 0 or phi == M_PI or theta == 0 or theta == M_PI)
	{
		if (y0 < y-dy or y0 > y+dy)
			return Intersections{No<double>(), No<double>()};
			
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
	if (ymin > track.getEnd() or ymax < track.getStart())
		return Intersections{No<double>(), No<double>()};

	smin = max({xmin, ymin, zmin});
	smax = min({xmax, ymax, zmax});
	
	if (smin > track.getStart())
		points.enter = smin;
	else
		points.enter = No<double>();
		
	if (smax < track.getEnd())
		points.exit = smax;
	else
		points.exit = No<double>();
		
	//if(points.enter and points.exit)
	//	cout << *points.enter << " " << *points.exit << endl;
	
	return points;
}


double ActiveObject::SpaceTravelled (const Track & track, const Interaction& inter) const
{
	auto points = intersectionPoints(track, inter);
	
	double start = track.getStart();
	double end = track.getEnd();
	
	double Max = points.exit.value_or(end);
	double Min = points.enter.value_or(start);

	if (not(Max > Min))
		return 0;

	return (Max - Min);
}

bool ActiveObject::isInside(const Track& t) const
{
	double z0 = t.pos.z;
	double theta = t.dir.theta;
	double z = zposition;
	double dz = zlen *.5;
	double z1 = (z-z0-dz)/(cos(theta));
	double z2 = (z-z0+dz)/(cos(theta));
	double zmin = std::min(z1, z2);
	double zmax = std::max(z1, z2);
	
	if (zmin > t.getEnd() or zmax < t.getStart())
		return false;
	
	return true;
}

void ActiveObject::interact(Track& t, const Interaction& inter) const
{
	setInteractionPoints(t, inter);
	setCharge(t, inter);
	setDecayPoint(t, inter);
}

void ActiveObject::setInteractionPoints(Track& t, const Interaction& inter) const
{
	if (inter.intersectionsComputed(ID))
		return;
		
	auto points = intersectionPoints(t, inter);
	
	inter.addIntersectionPoint(ID, points);
}

void ActiveObject::setCharge(Track& t, const Interaction& inter) const
{
	if (inter.chargeComputed(ID))
		return;
	
	auto charge = getCharge(t, inter);
	
	inter.addCharge(ID, charge);
}

void ActiveObject::setDecayPoint(Track& t, const Interaction& inter) const
{
	t.setEnd(getDecayPoint(t, inter));
}
	
