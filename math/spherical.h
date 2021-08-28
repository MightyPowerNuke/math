#pragma once


#include <cassert>
#include <ostream>

#include "math.h"
#include "vector.h"

namespace mpn
{

	/* 
	Stores a spherical coordinate (3D vector). 
	The angles are assumed to be in radians.
	*/
	template<typename T>
	class SphericalVector {
		static constexpr int N = 3;
		static constexpr size_t arraySize = N * sizeof(T);

	public:
		SphericalVector() : v{ T(0), T(0), T(0) } {}
		SphericalVector(const SphericalVector& c) {
			memcpy(v, c.v, arraySize);
		}
		
		/*explicit SphericalVector(T _v[N]) {
			throw exc::UnsupportedOperationException();
		}
		explicit SphericalVector(const T _v[N]) {
			throw exc::UnsupportedOperationException();
		}*/

		SphericalVector(T r, T theta, T phi) {
			if (r < T(0)) 
				throw std::invalid_argument("Radius is negative");
			if (r == T(0) && (theta != T(0) || phi != T(0)))
				throw std::invalid_argument("Spherical vector angles must be zero if the radius is zero");

			v[0] = r;

			if (theta == T(PI_2))
			{
				v[1] = T(PI_2);
				v[2] = T(0);
			}
			else if (theta == T(-PI_2))
			{
				v[1] = T(-PI_2);
				v[2] = T(0);
			}
			else
			{
				//Enforcing constraints on vector values
				const T signOfCosTheta = cosf(theta) >= 0 ? T(+1) : T(-1);

				//If the inclination angle is in the wrong hemisphere,
				//we correct by adding 180° to the direction angle.
				if (signOfCosTheta < 0) phi += PI;
				
				//TODO fmodf works against template parameter)
				v[1] = fmodf(theta, T(PI_2));
				v[2] = fmodf(phi, T(PI * 2));
				if (v[2] < 0) v[2] += PI * 2;
			}
		}

		T operator[](int index) const {
			assert(index >= 0 && index < N);
			return v[index];
		}

		//Being able to modify indexed values would make it impossible
		//to enforce range checks.
		/*T& operator[](int index) {
			assert(index >= 0 && index < N);
			return v[index];
		}*/

		SphericalVector<T>& operator=(const SphericalVector<T>& rhs) {
			memcpy(v, rhs.v, arraySize);
			return *this;
		}
		
		float length() const {
			return v[0];
		}

	private:
		T /*alignas(16)*/ v[N];
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const SphericalVector<T>& vector) {
		out << '(' << vector[0] << ',' << vector[1] << ',' << vector[2] << ')';
		return out;
	}

	template<typename T>
	bool operator==(const SphericalVector<T>& lhs, const SphericalVector<T>& rhs) {
		for (int i = 0; i < 3; ++i)
			if (abs(lhs[i] - rhs[i]) > 1e-5f)
				return false;
		return true;
	}

	template<typename T>
	bool operator!=(const SphericalVector<T>& lhs, const SphericalVector<T>& rhs) {
		return !(lhs == rhs);
	}

	using SphericalVector3 = SphericalVector<float>;
}
