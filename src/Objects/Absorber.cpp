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

using namespace std;

Absorber::Absorber(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen, double prob):
  ActiveObject(zheight, xdisplace, ydisplace, xlen, ylen, zlen),
  prob ( prob )
{ 
	if (prob == 0)
		this->prob = 20;
}

Absorber::~Absorber()
  { }

void Absorber::SetProb(double probability)
{
  prob = probability;
  return;
}

double Absorber::GetAbsorptionLenght() const
{
  std::uniform_real_distribution<double> dis(0, prob);		// genera la posizione in cui si ferma il raggio cosmico
  
  return dis(gen());
}

Option<double> Absorber::Absorbing_ornot(const Track & ray, const Interaction& inter) const 
{

  unsigned int abs_number = 0;
  
  double abs_lenght =       GetAbsorptionLenght();
  double space_travelled =  SpaceTravelled(ray, inter);
  if(abs_lenght < space_travelled) abs_number = 1;
  
  if(abs_number == 0)
  {
    return No<double>();
  }
  
  auto enter = intersectionPoints(ray, inter).enter;
   
  
  return (enter?*enter:ray.getStart()) + abs_lenght;
}

double Absorber::getCharge(const Track& t, const Interaction& inter) const
{
	return 0;
}

double Absorber::getDecayPoint(const Track& t, const Interaction& inter) const
{
	auto endPoint = Absorbing_ornot(t, inter);
	
	if (endPoint)
	{
		return *endPoint;
	}
	return numeric_limits<double>::max();
}
