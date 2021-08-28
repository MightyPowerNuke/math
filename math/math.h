#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

#undef min
#undef max

constexpr float PI = static_cast<float>(M_PI);
constexpr float PI_2 = static_cast<float>(M_PI_2);
constexpr float PI_4 = static_cast<float>(M_PI_4);

namespace mpn {

	constexpr float EPSILON = 1e-5f; //float error range
	
	/*Solves quadric formula in the set of real numbers.
	 - A,B,C: quadric constants. Discriminant is calculated locally.
	 - res1,res2: results.
	 D = Discriminant
	 If D>0, both results hold correct values.
	 If D=0, res1 holds the only solution and res2 is NaN.
	 If D<0, both res1 and res2 are returned as NaN.*/
	void solvequadric(float A, float B, float C, float& res1, float& res2) noexcept;

	/*Solves quartic formula in the set of real numbers.
	 - A,B,C,D,E: quartic constants. Discriminant is calculated locally.
	 - res1,res2,res3,res4: results.
	 No discriminant information can be relied on. Non-real roots should be NaN, but that is not guaranteed.*/
	void solvequartic(float A, float B, float C, float D, float E, float& res1, float& res2, float& res3, float& res4) noexcept;

	/*Returns the smaller non-negative value out of 'a' and 'b'. (By non-negative we mean it is greater than epsilon.)
	  If both values are smaller than epsilon, returns a negative value with no specific meaning.*/
	float smallerNonnegative(float a, float b) noexcept;

	/*Checks whether the value is between the bounds inclusive.*/
	template<typename T>
	constexpr bool between(T value, T lowerBound, T upperBound) noexcept
	{
		return value >= lowerBound && value <= upperBound;
	}

	template<>
	constexpr bool between(float value, float lowerBound, float upperBound) noexcept
	{
		return value >= lowerBound - EPSILON && value <= upperBound - EPSILON;
	}
}
