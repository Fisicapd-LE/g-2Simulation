/*
 * Track.cpp
 *
 *  Created on: 21/lug/2016
 *      Author: enrico
 */

#include "Track.h"

#include <limits>
#include <iostream>

using namespace std;

Track::Track(Position3D p, Direction d, Spin s, Flavour f, double energy, double zStart, Time t)
	:pos(p), dir(d), s(s), f(f), t(t), energy(energy), magic(0), start(zStart), end(numeric_limits<double>::max())
{
}

Track::~Track()
{
}

std::unique_ptr<Track> Track::generate()
{
	Position3D p = generatePos();
	Direction d = generateDir();
	Flavour f = generateFlavour();
	Spin s = generateSpin(f);
	double energy = generateMuonEnergy();
	//while(energy < 0)
	{
		energy = generateMuonEnergy();
	}
	//cout << energy << endl;
	return unique_ptr<Track>(new Track(p, d, s, f, energy, -numeric_limits<double>::max()));
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

Direction Track::generateDir()
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
	}while(uniNum > (cos(cosNum)*cos(cosNum)));	// genera theta, con algoritmo hit or miss
	
	dir.theta = M_PI - cosNum;

	return dir;
}

Track::Spin Track::generateSpin(Flavour f)
{
	std::uniform_real_distribution<double> dis(0, 1);
	
	if (dis(gen()) < polarization() + 0.5)
		return (f == Flavour::muP)?Spin::b:Spin::f;
	
	return (f == Flavour::muP)?Spin::f:Spin::b;
}	

Track::Flavour Track::generateFlavour()
{
	std::uniform_real_distribution<double> dis(0, 1);
	
	if (dis(gen()) < 1/(1+muRatio()))
		return Flavour::muN;
		
	return Flavour::muP;
}

double Track::generateMuonEnergy()
{
	const double attenuation = 0.54;
	
	const double normg = 1.7/2.7;			// normalization of the g(x)
	const double normf = 1./0.09597895657;	// normalization of the f(x)
	
	const double maxh = normf/normg;
	
	// result
	double E;
	
	// variables controlling the loop
	double h;
	double y;
	
	// Von Neumann rejection method
	do
	{
		double x = generate_canonical<double, 16>(gen());
		
		double g;
	
		if (x < normg)
		{
			//cout << uniNum/norm << endl;
			E = x/normg;
			
			//cout << E << endl;
			
			g = normg;
		}
		else
		{
			E = pow(1./(2.7*(1-x)), 1/1.7);
			
			g = normg*pow(E, -2.7);
		}
		//cout << E << endl;
		
		double f = (1 - 7.89896e-03/(pow(x, 1.15)+7.89896e-03))*
			normf/(50 + pow(E + attenuation, 2.7));
		
		h = f/g;
		
		y = generate_canonical<double, 16>(gen())*maxh;
	}while(y > h);
	
	//E -= attenuation; 
	
	//cout << E << endl;
	
	return E;
}


