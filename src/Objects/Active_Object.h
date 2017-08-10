/*
 * Active_Object.h
 *
 *  Created on: 01/ago/2017
 *      Author: davide
 */
//class representing an object inside the solenoid. It contains all its characteristics, in particular
//its position and its shape and dimensions. It will describe both scintillators and absorber, who will
//inherit from this class.

#ifndef ACTIVE_OBJECT_H_
#define ACTIVE_OBJECT_H_

#include "Utilities/Vector3D.h"
#include "Utilities/Generator.h"


class Track;

class Active_Object: public Generator
{
public:
  //   Active_Object();
  //   Active_Object(double zheight);
  //   Active_Object(double zheight, double xdisplace, double ydisplace);
  Active_Object(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15);
  
  virtual ~Active_Object();
  
  //to read object data
  double GetZposition() const 		{return zposition;};
  double GetXlen() const 			{return xlen;};
  double GetYlen() const 			{return ylen;};
  double GetZlen() const 			{return zlen;};
  double GetXdisplace() const 		{return xdisplace;};
  double GetYdisplace() const 		{return ydisplace;};
//   double GetZdisplace() const 		{return zdisplace;};

  Position3D GetEnteringPoint() const	{return entering_point;};
  //Position3D GetExitPoint() const	{return exit_point;};
  
  void SetZposition(double z);
  
  
protected:
  
  double zposition;
  double xlen;
  double ylen;
  double zlen;
  double xdisplace;
  double ydisplace;
//   double zdisplace;
  
  mutable Position3D entering_point;
  mutable Position3D exit_point;
  
  double SpaceTravelled (const Track & ray, Position3D * start = nullptr) const;	// function to compute the space cosmic ray travels inside 
																					// the active object before going out
																					// IT DOES NOT CONSIDERATE INTERACTION WITH MATTER
  
  
};

#endif /* ACTIVE_OBJECT_H_ */ 
