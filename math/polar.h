#pragma once


#include <cassert>
#include <ostream>

#include "vector.h"

namespace mpn
{

	/*
	Stores a polar coordinate (2D vector).
	The angle is assumed to be in radians.
	*/
	template<typename T>
	class PolarVector {
		static const int N = 2;
		static const size_t arraySize = N * sizeof(T);

	public:
		PolarVector() {
			for (int i = 0; i < N; ++i)
				v[i] = T(0);
		}
		PolarVector(const PolarVector& c) {
			memcpy(v, c.v, arraySize);
		}
		explicit PolarVector(T _v[N]) {
			//TODO range checking
			memcpy(v, _v, arraySize);
		}
		explicit PolarVector(const T _v[N]) {
			//TODO range checking
			memcpy(v, _v, arraySize);
		}

		PolarVector(T r, T theta) {
			v[0] = r;
			v[1] = fmodf(theta, 2.0f*PI);
			if (v[1] < 0.0f) v[1] += 2.0f*PI;
		}

		T operator[](int index) const {
			assert(index >= 0 && index < N);
			return v[index];
		}
		T& operator[](int index) {
			assert(index >= 0 && index < N);
			return v[index];
		}

		PolarVector<T>& operator=(const PolarVector<T>& rhs) {
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
	std::ostream& operator<<(std::ostream& out, const PolarVector<T>& vector) {
		out << '(' << vector[0] << ',' << vector[1] << ')';
		return out;
	}

	template<typename T>
	bool operator==(const PolarVector<T>& lhs, const PolarVector<T>& rhs) {
		for (int i = 0; i < 2; ++i)
			if (abs(lhs[i] - rhs[i]) > 1e-5f)
				return false;
		return true;
	}

	template<typename T>
	bool operator!=(const PolarVector<T>& lhs, const PolarVector<T>& rhs) {
		return !(lhs == rhs);
	}

	typedef PolarVector<float> PolarVector2;
}
