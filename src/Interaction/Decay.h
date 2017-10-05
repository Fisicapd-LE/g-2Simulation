/*
 * Decay.h
 *
 *  Created on: 09/ago/2017
 *      Author: enrico
 */


#ifndef DECAY_H_
#define DECAY_H_

#include "Tracks/Track.h"

#include "Utilities/Time.h"
#include "Utilities/Vector3D.h"

#include "Configuration/B.h"

#include <memory>

class Decay: public Generator
{
	public:
		Decay() = default;
		Decay(Decay&) = delete;
		Decay& operator=(Decay&) = delete;
		virtual ~Decay() {};
		
		static std::unique_ptr<Track> decay(const Track& cosmic, Position3D pos, const BGen& bg);
		
	private:
		
		static constexpr long slowDec = 2197; 	// ns
		static constexpr long fastDec = 160;	// ns
		
		static Direction generateElecDir();
		static Time generateDecayTime(Track::Flavour f);
		
		static Direction generateUniformDir();
		
		static constexpr double a = 1./3.;
};

#endif /* ACTIVE_OBJECT_H_ */ 

