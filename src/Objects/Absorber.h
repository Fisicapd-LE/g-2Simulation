/*
 * Absorber.h
 *
 *  Created on: 01/ago/2017
 *      Author: davide
 */

#ifndef ABSORBER_H_
#define ABSORBER_H_

#include "ActiveObject.h"

#include <TGraph.h>

#include <string>


class Absorber: public ActiveObject
{
	public:
		Absorber(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 25);

		virtual ~Absorber();

		virtual double getCharge(double start, double end) const override;
		virtual double getRange(double energy, Track::Flavour f, double& magicNumber) const override;
		virtual double getEnergyLoss(double energy, Track::Flavour f, double length, double& magicNumber) const override;
		
	private:
		static TGraph* electronRange();
		static TGraph* electronStoppingPower();
		
		static TGraph* muonRange();
		static TGraph* muonStoppingPower();
		
		double byStepEnergyLoss(double energy, double length, TGraph* stoppingPower) const;
};

#endif /* ABSORBER_H_ */

