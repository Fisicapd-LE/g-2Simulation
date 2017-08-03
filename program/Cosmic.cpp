/*
 * Cosmic.cpp
 *
 *  Created on: 21/lug/2016
 *      Author: enrico
 */

#include "Cosmic.h"

using namespace std;

Cosmic::Cosmic(Position p, Direction d)
	:pos(p), dir(d)
{

}

Cosmic::~Cosmic()
{

}

std::unique_ptr<Cosmic> Cosmic::generate()
{
	Position p = generatePos();
	Direction d = generateDir();
	return unique_ptr<Cosmic>(new Cosmic(p, d));
}

Cosmic::Position Cosmic::generatePos()
{
	std::uniform_real_distribution<double> disx(-350, 350);	// genera la posizione x in mm sul rilevatore più esterno
	std::uniform_real_distribution<double> disy(-175, 175); // genera la posizione y in mm sul rilevatore più esterno

	Position pos;
	pos.x = disx(gen());
	pos.y = disy(gen());

	return pos;
}

Cosmic::Direction Cosmic::generateDir()
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

