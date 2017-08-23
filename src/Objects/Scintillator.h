/*
 * Scintillator.h
 *
 *  Created on: 23/mag/2017
 *      Author: davide
 */

#ifndef SCINTILLATOR_H_
#define SCINTILLATOR_H_

#include "ActiveObject.h"
#include "Tracks/Track.h"
#include <vector>

using namespace std;



class Scintillator: public ActiveObject
{
public:
  Scintillator(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15, short int mode = 8, double width = 10);
  
  virtual ~Scintillator();
  
  //to read scintillator data use the Active_Object format
  double GetConv_ratio() const 			{return conv_ratio;};
  double GetMode() const 			{return mode;};
  double GetWidth() const 			{return width;};
  
  void SetConv_ratio(double ratio);
  
  void interact(const Track& t, const Interaction& inter) const override;
  
private:
  double conv_ratio;
  double mode;
  double width;
  
  double GetSingleCharge(const Track & ray) const;
};

#endif /* SCINTILLATOR_H_ */

