/*
 * Track.cpp
 *
 *  Created on: 21/lug/2016
 *      Author: enrico
 */

#include "Track.h"

using namespace std;

Track::Track(Position p, Direction d, Spin s, Flavour f, double t = 0)
	:pos(p), dir(d), s(s), f(f), time(t)
{
}

Track::~Track()
{
}

std::unique_ptr<Track> Track::generate()
{
	Position p = generatePos();
	Direction d = generateDir();
	Spin s = generateSpin();
	Flavour f = generateFlavour();
	return unique_ptr<Track>(new Track(p, d, s, f));
}

Track::Position Track::generatePos()
{
	std::uniform_real_distribution<double> disx(-350, 350);	// genera la posizione x in mm sul rilevatore più esterno
	std::uniform_real_distribution<double> disy(-175, 175); // genera la posizione y in mm sul rilevatore più esterno

	Position pos;
	pos.x = disx(gen());
	pos.y = disy(gen());

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

