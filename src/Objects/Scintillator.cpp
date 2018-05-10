/*
 * Scintillator.cpp
 *
 *  Created on: 23/mag/2017
 *      Author: Davide
 */

// #include "Discriminator.h"
// #include "Charge.h"



#include "Scintillator.h"

#include <iostream>
#include <limits>

using namespace std;

Scintillator::Scintillator(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen, short int mode, double width)
	:ActiveObject(zheight, xdisplace, ydisplace, xlen, ylen, zlen),
	mode( mode ),
	width( width )
{
	conv_ratio = mode / width;
}

Scintillator::~Scintillator()
{ }

void Scintillator::setConvRatio(double ratio)
{
	conv_ratio = ratio;
	return;
}

double Scintillator::getCharge(double start, double end) const
{
	return (end - start)*conv_ratio;
}

double Scintillator::getRange(double energy, Track::Flavour f, double& magicNumber) const
{
	if ( f == Track::Flavour::muP or f == Track::Flavour::muN)
		return numeric_limits<double>::max();
		
	if (magicNumber == 0)
	{
		magicNumber = 	2*		// mm/MeV
						1000*	// MeV/GeV
						energy;	// GeV
	}
	return 	magicNumber;
}

double Scintillator::getEnergyLoss(double, Track::Flavour, double length, double& magicNumber) const
{
	magicNumber -= length;
	return 0;
}

