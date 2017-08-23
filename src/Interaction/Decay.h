/*
 * Decay.h
 *
 *  Created on: 09/ago/2017
 *      Author: enrico
 */


#ifndef DECAY_H_
#define DECAY_H_

#include "Tracks/Track.h"

#include "Utilities/Vector3D.h"

#include <memory>

class Decay: public Generator
{
	public:
		Decay() = default;
		Decay(Decay&) = delete;
		Decay& operator=(Decay&) = delete;
		virtual ~Decay() {};
		
		static std::unique_ptr<Track> decay(const Track& cosmic, Position3D pos);
		
	private:
		class BGen
		{
			public:
				B operator()(Position3D p) const;
		};
		static BGen bg()
		{
			static BGen bg;
			return bg;
		}
		
		static constexpr long slowDec = 2200; 	// ns
		static constexpr long fastDec = 1;		// ns
		
		static Direction generateElecDir();
		static double generateDecayTime(Track::Flavour f);
		
		static constexpr double a = 1./3.;
};

#endif /* ACTIVE_OBJECT_H_ */ 

