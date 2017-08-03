/*
 * Absorbing_ornot.cpp
 *
 *  Created on: 02/ago/2017
 *      Author: davide
 */

#include "Cosmic.h"
#include "Position3D.h"
#include "Absorber.h"

Position3D * Absorbing_ornot(const Absorber * abs0, const Cosmic & ray)
{
  double abs_lenght;
  double space_travelled;

  unsigned int abs_number = 0;

//   for(unsigned int i = 0; i < abs_vec.size(); i++)
//   {
//     abs_lenght =       (abs_vec.at(i))->GetAbsorptionLenght(ray); 	//get the lenght the cosmic is absorbed
//     space_travelled =  (abs_vec.at(i))->SpaceTravelled(ray);		//get the lenght the cosmic stays insideabsorber if not absorbed
//     if(abs_lenght < space_travelled) abs_number = i; 			//the cosmic is absorbed by i-th absorber
//   }
  
  abs_lenght =       (abs0)->GetAbsorptionLenght(ray);
  space_travelled =  (abs0)->SpaceTravelled(ray);
  if(abs_lenght < space_travelled) abs_number = 1;
  
  
  if(abs_number == 0)
  {
    return nullptr;
  }
  
//   Absorber * absorbed = abs_vec.at(abs_number);

  Position3D * interaction_point = new Position3D;
  Position3D entering_point;
  
  
  entering_point.x = abs0->GetEnteringPoint()->x;	//prima di poterlo fare devo aver calcolato lo SpaceTravelled(ray)
  entering_point.y = abs0->GetEnteringPoint()->y;
  entering_point.z = abs0->GetEnteringPoint()->z;
  
  double x0,y0,phi,theta;
  x0 = ray.pos.x;
  y0 = ray.pos.y;
  phi = ray.dir.phi;
  theta = ray.dir.theta;
  
  
  interaction_point->x = entering_point.x					//prima parte data dal punto X d'interazione nell'assorbitore
		       + abs_lenght * sin(theta) * cos(phi);		//seconda parte data dal progredire all'interno dell'assorbitore
		      
  interaction_point->y = entering_point.y					//prima parte data dal punto Y d'interazione nell'assorbitore
		       + abs_lenght * sin(theta) * sin(phi);		//seconda parte data dal progredire all'interno dell'assorbitore  
		      
  interaction_point->z = entering_point.z					//prima parte data dal punto Z d'interazione nell'assorbitore
		       + abs_lenght * cos(theta);				//seconda parte data dal progredire all'interno dell'assorbitore


  return interaction_point;
}
