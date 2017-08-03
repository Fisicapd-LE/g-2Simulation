/*
 * Absorber.h
 *
 *  Created on: 01/ago/2017
 *      Author: davide
 */

#ifndef ABSORBER_H_
#define ABSORBER_H_

#include "Active_Object.h"
#include <string>


class Absorber: public Active_Object
{
public:
//   Absorber();
//   Absorber(double zheight);
//   Absorber(double zheight, double xdisplace, double ydisplace);
  Absorber(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15, double prob = 1);
  
  virtual ~Absorber();
  
  //to read absorber data use the Active_Object format
  double GetProb() const 			{return prob;};
  
  double GetAbsorptionLenght(const Track & ray) const;
  void SetProb(double prob);
  Position3D Absorbing_ornot(const Track & ray) const;
  
private:
  
  double prob; 
  /*Si considera la probabilità di interazione come uniforme all'interno dell'assorbitore.
   *questo parametro indica proprio quanto vale questa probabilità di assorbimento*/
  
  
};

#endif /* ABSORBER_H_ */

