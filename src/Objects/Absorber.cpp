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

Absorber::Absorber(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen, double prob):
  ActiveObject(zheight, xdisplace, ydisplace, xlen, ylen, zlen),
  prob ( prob )
  { }

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
  
  double phi = ray.dir.phi;
  double theta = ray.dir.theta;
  
  return intersectionPoints(ray, inter).enter.valueOr(ray.getStart());
}

void interact(const Track& t, const Interaction& inter) const
{
	inter.setCharge(ID, 0);
		
	auto endPoint = Absorbing_ornot(ray, inter);
	
	if (endPoint)
		ray.setEnd(*endPoint);
}
