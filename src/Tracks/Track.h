/*
 * Track.h
 *
 *  Created on: 21/lug/2016
 *      Author: enrico
 */

#ifndef TRACK_H_
#define TRACK_H_

#include "Utilities/Generator.h"
#include "Utilities/Vector3D.h"
#include "Utilities/Time.h"

#include "Option/Option.h"

#include <memory>

class Decay;

class Track: public Generator
{
	friend Decay;
	public:
		virtual ~Track();
		
		enum struct Spin: char
		{
			b = -1,
			f = 1,
			n = 0
		};
		
		enum class Flavour
		{
			muP = 0,
			muN,
			eP,
			eN
		};

		Position3D pos;
		Direction dir;
		Spin s;
		Flavour f;
		Time t;
		
		double getStart() const
		{
			return start;
		};
		double getEnd() const
		{
			return end;
		}
		
		void setEnd(double end)
		{
			if (end < this->end and end > this->start)
				this->end = end;
		};
		
		static std::unique_ptr<Track> generate();
		
		Position3D getPosition(double s) const
		{
			return Position3D{
				pos.x + s*sin(dir.theta)*cos(dir.phi),
				pos.y + s*sin(dir.theta)*sin(dir.phi),
				pos.z + s*cos(dir.theta)
			};
		};
		
	private:
		Track(Position3D p, Direction d, Spin s, Flavour f, double zStart, Time t = 0);
		
		const double start;
		double end;

		static Position3D generatePos();
		static Direction generateDir();
		static Spin generateSpin(Flavour f);
		static Flavour generateFlavour();
		
		static double polarization()
		{
			return 0.2;
		};
		
		static double muRatio()
		{
			return 1.25;
		}
};

#endif /* Track_H_ */


