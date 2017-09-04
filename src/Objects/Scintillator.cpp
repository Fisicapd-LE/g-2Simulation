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

using namespace std;

Scintillator::Scintillator(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen, short int mode, double width):
  ActiveObject(zheight, xdisplace, ydisplace, xlen, ylen, zlen),
  mode( mode ),
  width( width )
  {
    conv_ratio = mode / width;
  }

Scintillator::~Scintillator()
  { }

void Scintillator::SetConv_ratio(double ratio)
{
  conv_ratio = ratio;
  return;
}


double Scintillator::GetSingleCharge(const Track & ray, const Interaction& inter) const
{
  //double ratio = conv_ratio;
  return SpaceTravelled(ray, inter) * conv_ratio;
}

double Scintillator::getCharge(const Track& t, const Interaction& inter) const
{
	double charge = GetSingleCharge(t, inter);
	return charge;
}

double Scintillator::getDecayPoint(const Track& t, const Interaction& inter) const
{
	return numeric_limits<double>::max();
}

