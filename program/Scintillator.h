/*
 * Scintillator.h
 *
 *  Created on: 23/mag/2017
 *      Author: davide
 */

#ifndef SCINTILLATOR_H_
#define SCINTILLATOR_H_

#include "Active_Object.h"
#include "Cosmic.h"

class Scintillator: public Active_Object
{
public:
//   Scintillator();
//   Scintillator(double zheight);
//   Scintillator(double zheight, double xdisplace, double ydisplace);
  Scintillator(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15, short int mode = 8, double width = 10);
  
  virtual ~Scintillator();
  
  //to read scintillator data use the Active_Object format
  double GetConv_ratio() const 			{return conv_ratio;};
  double GetMode() const 			{return mode;};
  double GetWidth() const 			{return width;};
  
  void SetConv_ratio(double ratio);
  
  double GetSingleCharge(const Cosmic & ray) const;
  
private:
  
  double conv_ratio;
  double mode;
  double width;
  
};

#endif /* SCINTILLATOR_H_ */

