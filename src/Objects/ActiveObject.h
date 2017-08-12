/*
 * ActiveObject.h
 *
 *  Created on: 01/ago/2017
 *      Author: davide
 */
//class representing an object inside the solenoid. It contains all its characteristics, in particular
//its position and its shape and dimensions. It will describe both scintillators and absorber, who will
//inherit from this class.

#ifndef ActiveObject_H_
#define ActiveObject_H_

#include "Utilities/Vector3D.h"
#include "Utilities/Generator.h"

#include <limits>

class Track;

class ActiveObject: public Generator
{
public:
  //   ActiveObject();
  //   ActiveObject(double zheight);
  //   ActiveObject(double zheight, double xdisplace, double ydisplace);
  ActiveObject(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15);
  
  virtual ~ActiveObject();
  
  //to read object data
  double GetZposition() const 		{return zposition;};
  double GetXlen() const 			{return xlen;};
  double GetYlen() const 			{return ylen;};
  double GetZlen() const 			{return zlen;};
  double GetXdisplace() const 		{return xdisplace;};
  double GetYdisplace() const 		{return ydisplace;};
//   double GetZdisplace() const 		{return zdisplace;};
  
  void SetZposition(double z);
  
  std::pair<double, double> intersectionPoints(const Track& t, double zmax = std::numeric_limits<double>::quiet_NaN()) const;
  
protected:
  
  double zposition;
  double xlen;
  double ylen;
  double zlen;
  double xdisplace;
  double ydisplace;
//   double zdisplace;

  double SpaceTravelled (const Track & ray, double zmax = std::numeric_limits<double>::quiet_NaN()) const;	// function to compute the space cosmic ray travels inside 
																	// the active object before going out
																	// interaction with matter has to be exposed by setting the zmax
																	
  static int nObjects;
  int ID;
  


};

#endif /* ActiveObject_H_ */ 
