/*
 * Absorber.cpp
 *
 *  Created on: 01/ago/2017
 *      Author: Davide
 */

// #include "Discriminator.h"
// #include "Charge.h"

#include "Absorber.h"
#include <random>

Absorber::Absorber(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen, double prob):
  Active_Object(zheight, xdisplace, ydisplace, xlen, ylen, zlen),
  prob ( prob )
  { }

Absorber::~Absorber()
  { }

void Absorber::SetProb(double probability)
{
  prob = probability;
  return;
}

double Absorber::GetAbsorptionLenght(const Cosmic & ray) const
{
  double probability = prob;
  double path = SpaceTravelled(ray); 
  
  std::uniform_real_distribution<double> dis(0, prob);		// genera la posizione in cui si ferma il raggio cosmico
  double lenght = dis(gen());
  
  return lenght;
}