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

class Track
{
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

		Position pos;
		Direction dir;

		static std::unique_ptr<Track> generate();

	private:
		Track(Position p, Direction d);

		static Position generatePos();
		static Direction generateDir();
		
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
};

#endif /* Track_H_ */


