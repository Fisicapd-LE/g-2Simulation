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

double Absorber::GetAbsorptionLenght(const Track & ray) const
{
  double probability = prob;
  double path = SpaceTravelled(ray); 
  
  std::uniform_real_distribution<double> dis(0, prob);		// genera la posizione in cui si ferma il raggio cosmico
  double lenght = dis(gen());
  
  return lenght;
}

Position3D Absorber::Absorbing_ornot(const Track & ray) const 
{
  double abs_lenght;
  double space_travelled;

  unsigned int abs_number = 0;
  
  abs_lenght =       GetAbsorptionLenght(ray);
  space_travelled =  SpaceTravelled(ray);
  if(abs_lenght < space_travelled) abs_number = 1;
  
  Position3D interaction_point = {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
  
  if(abs_number == 0)
  {
    return interaction_point;
  }
  
  double phi,theta;
  phi = ray.dir.phi;
  theta = ray.dir.theta;
  
  Position3D entering_point = ray.getPosition(intersectionPoints(ray).first);
  
  
  interaction_point.x = entering_point.x					//prima parte data dal punto X d'interazione nell'assorbitore
		       + abs_lenght * sin(theta) * cos(phi);		//seconda parte data dal progredire all'interno dell'assorbitore
		      
  interaction_point.y = entering_point.y					//prima parte data dal punto Y d'interazione nell'assorbitore
		       + abs_lenght * sin(theta) * sin(phi);		//seconda parte data dal progredire all'interno dell'assorbitore  
		      
  interaction_point.z = entering_point.z					//prima parte data dal punto Z d'interazione nell'assorbitore
		       - abs_lenght * cos(theta);				//seconda parte data dal progredire all'interno dell'assorbitore


  return interaction_point;
}
