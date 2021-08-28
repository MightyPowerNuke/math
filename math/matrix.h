#pragma once


#include "vector.h"
#include "point.h"

#include <array>
#include <type_traits>

namespace mpn {

	/*Row-major matrix value class.*/
	template<int W, int H, typename T>
	class Matrix {
	private:
		static constexpr const int _arraySize = W * H;
	public:
		constexpr Matrix() {
			for (int i = 0; i < _arraySize; ++i)
				this->m[i] = T(0);
		}
		constexpr Matrix(const Matrix<W, H, T>& m) {
			for (int i = 0; i < _arraySize; ++i)
				this->m[i] = m.m[i];
		}
		constexpr explicit Matrix(T _m[_arraySize]) {
			for (int i = 0; i < _arraySize; ++i)
				this->m[i] = _m[i];
		}

		template<typename std::enable_if<_arraySize == 16>::type * = nullptr>
		constexpr explicit Matrix(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
			: m{ m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33 }
		{}


		constexpr T operator()(int row, int column) const {
			assert(row >= 0 && row < H && column >= 0 && column < W);
			return m[row + H * column];
		}
		constexpr T& operator()(int row, int column) {
			assert(row >= 0 && row < H && column >= 0 && column < W);
			return m[row + H * column];
		}

		constexpr Matrix<H, W, T> asTransposed() const {
			T result[_arraySize];
			for (int row = 0; row < H; ++row)
				for (int col = 0; col < W; ++col)
					result[row + H * col] = m[col + H * row];
			return Matrix<H, W, T>(result);
		}

		const T* const data() const {
			return m;
		}

	private:
		T m[_arraySize];
	};

	using Matrix4 = Matrix<4, 4, float>;
	using Matrix3 = Matrix<3, 3, float>;

	constexpr const Matrix4 identityMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	template<int W, int H, typename T>
	constexpr bool operator==(const Matrix<W, H, T>& left, const Matrix<W, H, T>& right)
	{
		for (int row = 0; row < H; ++row)
		{
			for (int column = 0; column < W; ++column)
			{
				if (left(row, column) != right(row, column))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<int W, int H>
	constexpr bool operator==(const Matrix<W, H, float>& left, const Matrix<W, H, float>& right)
	{
		for (int row = 0; row < H; ++row)
		{
			for (int column = 0; column < W; ++column)
			{
				if (abs(left(row, column) - right(row, column)) > 1e-5f) 
				{
					return false;
				}
			}
		}
		return true;
	}

	template<int W, int H, typename T>
	constexpr bool operator!=(const Matrix<W, H, T>& left, const Matrix<W, H, T>& right)
	{
		return !(left == right);
	}

	template<int W, int H, typename T>
	constexpr Matrix<W, H, T> operator*(const Matrix<W, H, T>& lhs, const Matrix<W, H, T>& rhs) {
		T res[W*H];
		for (int r = 0; r < 4; ++r)
			for (int c = 0; c < 4; ++c) {
				res[r + c*W] = T(0);
				for (int k = 0; k < 4; ++k)
					res[r + c*W] += lhs(r, k)*rhs(k, c);
			}
		return Matrix<W, H, T>(res);
	}

	template<int W, int H, typename T>
	constexpr Vector<H, T> operator*(const Vector<H, T>& v, const Matrix<W, H, T>& m) {
		T result[H];
		for (int i = 0; i < H; ++i)
			for (int j = 0; j < W; ++j)
				result[i] += v[j] * m(j, i);
		return Vector<H, T>(result);
	}
	

	template<int W, int H, typename T>
	constexpr Point<H, T> operator*(const Point<H, T>& p, const Matrix<W, H, T>& m) {
		T result[H];
		for (int i = 0; i < H; ++i)
			for (int j = 0; j < W; ++j)
				result[i] += p[j] * m(j, i);
		return Point<H, T>(result);
	}

	template<typename T>
	constexpr Point<3, T> operator*(const Point<3, T>& p, const Matrix<4, 4, T>& m) {
		const T multiplier = T(1) / (p[0] * m(0, 3) + p[1] * m(1, 3) + p[2] * m(2, 3) + 1.0f * m(3, 3)); //h=1 assumed here (p[3]=1)
		T result[3];
		result[0] = (p[0] * m(0, 0) + p[1] * m(1, 0) + p[2] * m(2, 0) + 1.0f * m(3, 0)) * multiplier;
		result[1] = (p[0] * m(0, 1) + p[1] * m(1, 1) + p[2] * m(2, 1) + 1.0f * m(3, 1)) * multiplier;
		result[2] = (p[0] * m(0, 2) + p[1] * m(1, 2) + p[2] * m(2, 2) + 1.0f * m(3, 2)) * multiplier;
		return Point<3, T>(result);
	}
	
	template<typename T>
	constexpr Vector<3, T> operator*(const Vector<3, T>& p, const Matrix<4, 4, T>& m) {
		const T h = (p[0] * m(0, 3) + p[1] * m(1, 3) + p[2] * m(2, 3));
		const T multiplier = h == T(0) ? T(1) : T(1) / h; //h=0 assumed here (v[3]=0), the fourth row of the matrix is not even considered
		T result[3];
		result[0] = (p[0] * m(0, 0) + p[1] * m(1, 0) + p[2] * m(2, 0)) * multiplier;
		result[1] = (p[0] * m(0, 1) + p[1] * m(1, 1) + p[2] * m(2, 1)) * multiplier;
		result[2] = (p[0] * m(0, 2) + p[1] * m(1, 2) + p[2] * m(2, 2)) * multiplier;
		return Vector<3, T>(result);
	}

	template<int W, int H, typename T>
	void loadIdentity(Matrix<W,H,T>& matrix) {
		for (int row = 0; row < H; ++row)
			for (int col = 0; col < W; ++col)
				matrix(row, col) = row == col ? T(1) : T(0);
	}

	template<int W, int H, typename T>
	std::ostream& operator<<(std::ostream& out, const Matrix<W, H, T>& matrix) {
		out << '(';
		for (int row = 0; row < H; ++row) {
			out << '{';
			for (int column = 0; column < W - 1; ++column)
				out << matrix(row, column) << ',';
			out << matrix(row, W - 1);
			out << '}';
		}
		out << ')';
		return out;
	}

}
