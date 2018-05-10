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

#include "Interaction/Interaction.h"

#include "Tracks/Track.h"

#include <limits>

class Track;

class ActiveObject: public Generator
{
	public:
		ActiveObject(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15);

		virtual ~ActiveObject();

		//to read object data
		double getZposition() const 		{return zposition;};
		double getXlen() const 				{return xlen;};
		double getYlen() const 				{return ylen;};
		double getZlen() const 				{return zlen;};
		double getXdisplace() const 		{return xdisplace;};
		double getYdisplace() const 		{return ydisplace;};

		void setZposition(double z);

		Option<Intersections> computeIntersectionPoints(const Track& t) const;
		
		
		virtual double getCharge(double start, double end) const = 0;
		virtual double getRange(double energy, Track::Flavour f, double& magicNumber) const = 0;
		virtual double getEnergyLoss(double energy, Track::Flavour f, double length, double& magicNumber) const = 0;
		
		int ID;

	protected:

		double zposition;
		double xlen;
		double ylen;
		double zlen;
		double xdisplace;
		double ydisplace;
															
		static int nObjects;
};

#endif /* ActiveObject_H_ */ 
