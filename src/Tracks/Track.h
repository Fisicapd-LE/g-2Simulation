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

#include <memory>

class Decay;

class Track: public Generator
{
	friend Decay;
	public:
		virtual ~Track();

		struct Direction
		{
			double phi;
			double theta;
		};
		
		enum struct Spin: char
		{
			d = -1,
			u = 1
		};
		
		enum class Flavour
		{
			muP,
			muN,
			eP,
			eN
		};

		Position3D pos;
		Direction dir;
		Spin s;
		Flavour f;
		double time;
		
		static std::unique_ptr<Track> generate();
		
		Position3D getPosition(double s) const
		{
			return Position3D{
				pos.x + s*sin(dir.theta)*cos(dir.phi),
				pos.y + s*sin(dir.theta)*sin(dir.phi),
				pos.z - s*cos(dir.theta)
			};
		};
		
		void addIntersectionPoint(int objectID, std::pair<double, double> points) const;
		std::pair<double, double> getIntersectionPoint(int objectID) const;

	private:
		Track(Position3D p, Direction d, Spin s, Flavour f, double t = 0);

		static Position3D generatePos();
		static Direction generateDir();
		static Spin generateSpin();
		static Flavour generateFlavour();
		
		static double polarization()
		{
			return 0.2;
		};
		
		static double muRatio()
		{
			return 1.25;
		}
		
		mutable std::array<std::pair<double, double>, 20> intersectionPoints;
};

#endif /* Track_H_ */


