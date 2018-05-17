/*
 * Absorber.cpp
 *
 *  Created on: 01/ago/2017
 *      Author: Davide
 */

// #include "Discriminator.h"
// #include "Charge.h"

#include "Absorber.h"

#include "Tracks/Track.h"

#include <random>
#include <limits>
#include <iostream>
#include <cmath>

#include <TCanvas.h>
#include <TH1.h>

const double cuDensity = 8.96;

using namespace std;

Absorber::Absorber(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen)
	:ActiveObject(zheight, xdisplace, ydisplace, xlen, ylen, zlen)
{
}

Absorber::~Absorber()
{}

double Absorber::getCharge(double, double) const
{
	return 0;
}

double Absorber::getRange(double energy, Track::Flavour f, double&) const
{
	//clog << energy << endl;
	if ( f == Track::Flavour::muP or f == Track::Flavour::muN)
	{
		//cout << energy << endl;
		return muonRange()->Eval(energy*1000)*10/cuDensity;
	}
		
	return electronRange()->Eval(energy*1000)*10/cuDensity;
}

double Absorber::getEnergyLoss(double energy, Track::Flavour f, double length, double&) const
{
	if ( f == Track::Flavour::muP or f == Track::Flavour::muN)
	{
#ifdef SLOPPY_ABSORBER
		return 0;
#else
		return byStepEnergyLoss(energy, length, muonStoppingPower());
#endif
	}
		
	return byStepEnergyLoss(energy, length, electronStoppingPower());
}

double Absorber::byStepEnergyLoss(double energy, double length, TGraph* stoppingPower) const
{
	int nSteps = 100;
	double step = length/nSteps;
	double loss = 0;
	for (int i = 0; i < nSteps; i++)
	{
		loss += stoppingPower->Eval((energy - loss)*1000) * step/10 * cuDensity / 1000;
	}
	return loss;
}

TGraph* Absorber::electronRange()
{
	static unique_ptr<TGraph> eR(new TGraph("electrons.txt", "%lg %*lg %lg"));
	return eR.get();
}

TGraph* Absorber::electronStoppingPower()
{
	static unique_ptr<TGraph> eSP(new TGraph("electrons.txt", "%lg %lg %*lg"));
	return eSP.get();
}

TGraph* Absorber::muonRange()
{
	static unique_ptr<TGraph> mR(new TGraph("muons.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg"));
	return mR.get();
}
TGraph* Absorber::muonStoppingPower()
{
	static unique_ptr<TGraph> mSP(new TGraph("muons.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %lg"));
	return mSP.get();
}

