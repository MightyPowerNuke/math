#pragma once

#include <cassert>
#include <ostream>

#include "random.h"

namespace mpn 
{

	template<int N, typename T>
	class Vector {
	public:
		constexpr Vector() {
			for (int i = 0; i < N; ++i)
				v[i] = T(0);
		}
		constexpr Vector(const Vector& c) {
			memcpy(v, c.v, arraySize);
		}
		constexpr explicit Vector(T _v[N]) {
			memcpy(v, _v, arraySize);
		}
		constexpr explicit Vector(const T _v[N]) {
			memcpy(v, _v, arraySize);
		}

		template<int N1 = N, typename = std::enable_if_t<N1 == 2>>
		constexpr Vector(T x, T y) : v{ x,y } {}

		template<int N1 = N, typename = std::enable_if_t<N1 == 3>>
		constexpr Vector(T x, T y, T z) : v{ x,y,z } {}

		constexpr T operator[](int index) const noexcept {
			assert(index >= 0 && index < N);
			return v[index]; 
		}
		constexpr T& operator[](int index) noexcept {
			assert(index >= 0 && index < N);
			return v[index];
		}

		constexpr Vector<N, T>& operator=(const Vector<N, T>& rhs) {
			memcpy(v, rhs.v, arraySize);
			return *this;
		}
		
		constexpr Vector<N, T>& operator*=(float rhs) {
			for (int i = 0; i < N; ++i)
				v[i] *= rhs;
			return *this;
		}

		constexpr float length() const {
			float result = 0.0f;
			for (int i = 0; i < N; ++i)
				result += v[i] * v[i];
			return sqrtf(result);
		}

		constexpr Vector<N, T> asUnitVector() const {
			float size = length();
			Vector<N, T> result;
			for (int i = 0; i < N; ++i)
				result[i] = v[i] / size;
			return result;
		}

		constexpr Vector<N, T>& operator+=(const Vector<N, T>& rhs)
		{
			for (int i = 0; i < N; ++i)
			{
				v[i] += rhs.v[i];
			}
			return *this;
		}

	private:
		static constexpr size_t arraySize = N * sizeof(T);
		T v[N];
	};

	using Vector3 = Vector<3, float>;
	using Vector2 = Vector<2, float>;

	template<int N, typename T>
	constexpr bool operator==(const Vector<N, T>& lhs, const Vector<N, T>& rhs) {
		for (int i = 0; i < N; ++i)
			if (abs(lhs[i] - rhs[i]) > 1e-5f)
				return false;
		return true;
	}

	template<int N, typename T>
	constexpr bool operator!=(const Vector<N, T>& lhs, const Vector<N, T>& rhs) {
		return !(lhs == rhs);
	}
	
	template<typename T>
	Vector<3, T> createRandom() {
		float x1, x2;
		do
		{
			x1 = realDistribution(randomEngine);
			x2 = realDistribution(randomEngine);
		} while (x1*x1 + x2*x2 >= 1.0f);
		const float sqrt = sqrtf(1.0f - x1*x1 - x2*x2);
		return Vector<3, T>(
			2.0f * x1 * sqrt,
			2.0f * x2 * sqrt,
			1.0f - 2.0f * (x1*x1 + x2*x2));
	}

	template<typename T>
	Vector<3, T> createRandom(T radius) {
		return createRandom<T>() * radius;
	}

	template<int N, typename T>
	constexpr Vector<N, T> operator+(const Vector<N, T>& lhs, const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] + rhs[i];
		return Vector<N, T>(result);
	}

	template<int N, typename T>
	constexpr Vector<N, T> operator-(const Vector<N, T>& lhs, const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] - rhs[i];
		return Vector<N, T>(result);
	}

	template<int N, typename T>
	constexpr Vector<N, T> operator-(const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = -rhs[i];
		return Vector<N, T>(result);
	}

	template<int N, typename T>
	constexpr T operator*(const Vector<N, T>& lhs, const Vector<N, T>& rhs) {
		T result = T(0);
		for (int i = 0; i < N; ++i)
			result += lhs[i] * rhs[i];
		return result;
	}

	template<>
	constexpr float operator*(const Vector<3, float>& lhs, const Vector<3, float>& rhs) {
		return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
	}

	template<int N, typename T>
	constexpr Vector<N, T> operator*(const Vector<N, T>& lhs, float rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] * rhs;
		return Vector<N, T>(result);
	}
	
	template<int N, typename T>
	constexpr Vector<N, T> operator*(float lhs, const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = rhs[i] * lhs;
		return Vector<N, T>(result);
	}

	template<int N, typename T>
	constexpr Vector<N, T> operator/(const Vector<N, T>& lhs, float rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] / rhs;
		return Vector<N, T>(result);
	}

	template<typename T> //cross product is only implemented for 3 dimensional vectors
	constexpr Vector<3, T> operator%(const Vector<3, T>& lhs, const Vector<3, T>& rhs) {
		return Vector<3, T>(lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2], lhs[0] * rhs[1] - lhs[1] * rhs[0]);
	}

	// Also known as Hadamard product
	template<int N, typename T>
	constexpr Vector<N, T> entrywiseProduct(const Vector<N, T>& lhs, const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] * rhs[i];
		return Vector<N, T>(result);
	}
	
	// Also known as Hadamard division
	template<int N, typename T>
	constexpr Vector<N, T> entrywiseDivision(const Vector<N, T>& lhs, const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] / rhs[i];
		return Vector<N, T>(result);
	}

	template<int N, typename T>
	std::ostream& operator<<(std::ostream& out, const Vector<N, T>& vector) {
		out << '(';
		for (int i = 0; i < N - 1; ++i)
			out << vector[i] << ',';
		out << vector[N - 1];
		out << ')';
		return out;
	}

}
