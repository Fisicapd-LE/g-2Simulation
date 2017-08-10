/*
 * Track.cpp
 *
 *  Created on: 21/lug/2016
 *      Author: enrico
 */

#include "Track.h"

#include <limits>

using namespace std;

Track::Track(Position3D p, Direction d, Spin s, Flavour f, double t)
	:pos(p), dir(d), s(s), f(f), time(t)
{
	for(auto& points: intersectionPoints)
		points = {numeric_limits<double>::quiet_NaN(),numeric_limits<double>::quiet_NaN()};
}

Track::~Track()
{
}

std::unique_ptr<Track> Track::generate()
{
	Position3D p = generatePos();
	Direction d = generateDir();
	Spin s = generateSpin();
	Flavour f = generateFlavour();
	return unique_ptr<Track>(new Track(p, d, s, f));
}

Position3D Track::generatePos()
{
	std::uniform_real_distribution<double> disx(-350, 350);	// genera la posizione x in mm sul rilevatore più esterno
	std::uniform_real_distribution<double> disy(-175, 175); // genera la posizione y in mm sul rilevatore più esterno

	Position3D pos;
	pos.x = disx(gen());
	pos.y = disy(gen());
	pos.z = 0;

	return pos;
}

Track::Direction Track::generateDir()
{
	std::uniform_real_distribution<double> dis(0, 2*M_PI);	// genera phi, uniforme

	Direction dir;
	dir.phi = dis(gen());
	double cosNum;
	double uniNum;
	do
	{
		cosNum = dis(gen())/4;
		uniNum = generate_canonical<double, 16>(gen());
	}while(uniNum > (4/M_PI)*(cos(cosNum)*cos(cosNum)));	// genera theta, con algoritmo hit or miss
	
	dir.theta = cosNum;

	return dir;
}

Track::Spin Track::generateSpin()
{
	std::uniform_real_distribution<double> dis(0, 1);
	
	if (dis(gen()) < (1 + polarization())/2)
		return Spin::u;
		
	return Spin::d;
}	

Track::Flavour Track::generateFlavour()
{
	std::uniform_real_distribution<double> dis(0, 1);
	
	if (dis(gen()) < 1/(1+muRatio()))
		return Flavour::muN;
		
	return Flavour::muP;
}

void Track::addIntersectionPoint(int objectID, std::pair<double, double> points) const
{
	intersectionPoints.at(objectID) = points;
}

std::pair<double, double> Track::getIntersectionPoint(int objectID) const
{
	return intersectionPoints.at(objectID);
}
