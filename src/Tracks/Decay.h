/*
 * Decay.h
 *
 *  Created on: 09/ago/2017
 *      Author: enrico
 */


#ifndef DECAY_H_
#define DECAY_H_

#include "Track.h"

#include "Utilities/Vector3D.h"

#include <memory>

class Decay: public Generator
{
	public:
		Decay() = default;
		Decay(Decay&) = delete;
		Decay& operator=(Decay&) = delete;
		virtual ~Decay() {};
		
		std::unique_ptr<Track> decay(std::unique_ptr<Track>&& cosmic, Position3D pos);
		
	private:
		class BGen
		{
			public:
				B operator()(Position3D p) const;
		};
		BGen bg;
		static constexpr double slowDec = 2200.; 	// ns
		static constexpr double fastDec = 1e-3;		// ns
		
		static Direction generateElecDir();
		static double generateDecayTime(Track::Flavour f);
		
		static constexpr double a = 1./3.;
};

#endif /* ACTIVE_OBJECT_H_ */ 

