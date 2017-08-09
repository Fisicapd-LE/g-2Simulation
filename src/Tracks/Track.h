/*
 * Track.h
 *
 *  Created on: 21/lug/2016
 *      Author: enrico
 */

#ifndef TRACK_H_
#define TRACK_H_

#include <memory>
#include <random>

class Decay;

class Track
{
	friend Decay;
	public:
		virtual ~Track();

		struct Position
		{
			double x;
			double y;
		};

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
		}

		Position pos;
		Direction dir;
		Spin s;
		Flovour f;
		double time;
		
		static std::unique_ptr<Track> generate();

	private:
		Track(Position p, Direction d, Spin s, Flavour f, double t);

		static Position generatePos();
		static Direction generateDir();
		static Spin generateSpin();
		static Flavour generateFlavour();
		
		static std::random_device& rd()
		{
			static std::random_device rd;
			return rd;
		};
		static std::mt19937_64& gen()
		{
			static std::mt19937_64 gen(rd()());
			return gen;
		};
		
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


