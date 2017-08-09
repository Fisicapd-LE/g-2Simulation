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
		BGen bg;
		const double slowDec = 2200.; 	// ns
		const double fastDec = 1e-3;	// ns
};

#endif /* ACTIVE_OBJECT_H_ */ 

