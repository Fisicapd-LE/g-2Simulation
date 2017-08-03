/*
 * Cosmic.h
 *
 *  Created on: 21/lug/2016
 *      Author: enrico
 */

#ifndef COSMIC_H_
#define COSMIC_H_

#include <memory>
#include <random>

class Cosmic
{
	public:
		virtual ~Cosmic();

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

		static std::unique_ptr<Cosmic> generate();

	private:
		Cosmic(Position p, Direction d);

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

#endif /* COSMIC_H_ */


