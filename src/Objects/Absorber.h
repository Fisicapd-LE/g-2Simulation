/*
 * Absorber.h
 *
 *  Created on: 01/ago/2017
 *      Author: davide
 */

#ifndef ABSORBER_H_
#define ABSORBER_H_

#include "ActiveObject.h"
#include <string>


class Absorber: public ActiveObject
{
public:
//   Absorber();
//   Absorber(double zheight);
//   Absorber(double zheight, double xdisplace, double ydisplace);
  Absorber(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15, double prob = 1);
  
  virtual ~Absorber();
  
  //to read absorber data use the Active_Object format
  double GetProb() const 			{return prob;};
  
  double GetAbsorptionLenght() const;
  void SetProb(double prob);
  Option<double> Absorbing_ornot(const Track & ray, const Interaction& inter) const;
  
  void interact(const Track& t, const Interaction& inter) const override;
  
private:
  
  double prob; 
  /*Si considera la probabilità di interazione come uniforme all'interno dell'assorbitore.
   *questo parametro indica proprio quanto vale questa probabilità di assorbimento*/
  
  
};

#endif /* ABSORBER_H_ */

