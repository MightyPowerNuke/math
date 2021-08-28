#pragma once


#include <type_traits>
#include <ostream>

#include "vector.h"

namespace mpn
{
	
	template<int N, typename T>
	class Point {
	public:
		constexpr Point() {
			for (int i = 0; i < N; ++i)	{
				p[i] = T(0);
			}
		}
		constexpr Point(const Point<N, T>& c) {
			memcpy(p, c.p, arraySize);
		}
		constexpr explicit Point(T _p[N]) {
			memcpy(p, _p, arraySize);
		}
		constexpr explicit Point(const Vector<N, T>& v) {
			for (int i = 0; i < N; ++i)
				p[i] = v[i];
		}
		
		template<int N1 = N, typename = std::enable_if_t<N1 == 1>>
		constexpr explicit Point(T x) : p{x} {}

		template<int N1 = N, typename = std::enable_if_t<N1 == 2>>
		constexpr Point(T x, T y) : p{ x,y } {}

		template<int N1 = N, typename = std::enable_if_t<N1 == 3>>
		constexpr Point(T x, T y, T z) : p{ x,y,z } {}

		constexpr Point<N, T>& operator=(const Point<N, T>& rhs) {
			memcpy(p, rhs.p, arraySize);
			return *this;
		}
		
		constexpr T operator[](int index) const { return p[index]; }
		constexpr T& operator[](int index) { return p[index]; }

		Vector<N, T> asVector() const {
			return Vector<N, T>(p);
		}

	private:
		static constexpr size_t arraySize = N * sizeof(T);
		T p[N];
	};

	using Point3 = Point<3, float>;
	using Point2 = Point<2, float>;
	using Point1 = Point<1, float>;

	template<int N ,typename T>
	constexpr bool operator==(const Point<N, T>& lhs, const Point<N, T>& rhs) {
		for (int i = 0; i < N; ++i)
			if (abs(lhs[i] - rhs[i]) > 1e-5f)
				return false;
		return true;
	}

	template<int N, typename T>
	constexpr bool operator!=(const Point<N, T>& lhs, const Point<N, T>& rhs) {
		return !(lhs == rhs);
	}

	template<int N, typename T>
	constexpr Point<N, T> operator+(const Point<N, T>& lhs, const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)	
			result[i] = lhs[i] + rhs[i];
		return Point<N, T>(result);
	}

	template<int N, typename T>
	constexpr Point<N, T> operator-(const Point<N, T>& lhs, const Vector<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] - rhs[i];
		return Point<N, T>(result);
	}

	template<int N, typename T>
	constexpr Vector<N, T> operator-(const Point<N, T>& lhs, const Point<N, T>& rhs) {
		T result[N];
		for (int i = 0; i < N; ++i)
			result[i] = lhs[i] - rhs[i];
		return Vector<N, T>(result);
	}

	template<int N, typename T>
	std::ostream& operator<<(std::ostream& out, const Point<N, T>& point) {
		out << '(';
		for (int i = 0; i < N - 1; ++i)
			out << point[i] << ',';
		out << point[N - 1];
		out << ')';
		return out;
	}

}