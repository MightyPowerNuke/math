#pragma once

#include <random>
#include <memory>

namespace mpn
{
	
	extern thread_local std::default_random_engine randomEngine;
	extern const std::uniform_real_distribution<float> realDistribution;
	extern const std::uniform_real_distribution<float> angleDistribution;

	/*Generates a random floating point number between 0 and 1. Uniform distribution.*/
	float frand();

	/*Generates a random floating point number between min and max. Uniform distribution.*/
	float frand(float min, float max);

	/*Generates a random integer in the interval of [min,max]. Uniform distribution.*/
	int dice(int min = 1.0f, int max = 6.0f);
}