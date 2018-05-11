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

#include <limits>
#include <iostream>
#include <cmath>

#include <TCanvas.h>

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
		return muonRange()->Eval(energy*1000)*10/8.92;
		
	return 	electronRange()->Eval(energy*1000)*10/8.92;
}

double Absorber::getEnergyLoss(double energy, Track::Flavour f, double length, double&) const
{
	if ( f == Track::Flavour::muP or f == Track::Flavour::muN)
		return byStepEnergyLoss(energy, length, muonStoppingPower());
		
	return byStepEnergyLoss(energy, length, electronStoppingPower());
}

double Absorber::byStepEnergyLoss(double energy, double length, TGraph* stoppingPower) const
{
	double step = length/10;
	double loss = 0;
	for (int i = 0; i < 10; i++)
	{
		loss += stoppingPower->Eval(energy*1000) * step/10 * 8.92 / 1000;
	}
	return loss;
}

TGraph* Absorber::electronRange()
{
	thread_local unique_ptr<TGraph> eR(new TGraph("electrons.txt", "%lg %*lg %lg"));
	return eR.get();
}

TGraph* Absorber::electronStoppingPower()
{
	thread_local unique_ptr<TGraph> eSP(new TGraph("electrons.txt", "%lg %lg %*lg"));
	return eSP.get();
}

TGraph* Absorber::muonRange()
{
	thread_local unique_ptr<TGraph> mR(new TGraph("muons.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg"));
	return mR.get();
}
TGraph* Absorber::muonStoppingPower()
{
	thread_local unique_ptr<TGraph> mSP(new TGraph("muons.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %lg"));
	return mSP.get();
}

