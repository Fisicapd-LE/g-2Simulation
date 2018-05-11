/*
 * Track.h
 *
 *  Created on: 09/Aug/17
 *      Author: enrico
 */
 
#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <random>

class Generator
{
	protected:
		static std::random_device& rd()
		{
			thread_local std::random_device rd;
			return rd;
		};
		static std::mt19937_64& gen()
		{
			thread_local std::mt19937_64 gen(rd()());
			return gen;
		};
};

#endif
