#pragma once

#include <array>

#include "vector.h"
#include "point.h"

namespace geom {

	/*Line value struct. Contains a point (origin point) P and a direction vector v.*/
	struct Line {
		::mpn::Point3 P;
		::mpn::Vector3 v;

		constexpr Line() noexcept : P(0,0,0), v(0,0,0) { }
		constexpr Line(const ::mpn::Point3& _P, const ::mpn::Vector3& _v) noexcept : P(_P), v(_v) {}
	};

	/*2D rectangle value class. Stores the top left and bottom right coordinates.*/
	template<typename T>
	class RectangleArea final {
	public:
		constexpr RectangleArea() : x0(-1), y0(-1), x1(+1), y1(+1) {}
		constexpr RectangleArea(T _x0, T _y0, T _x1, T _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1) {}
		constexpr T left() const { return x0; }
		constexpr T right() const { return x1; }
		constexpr T top() const { return y0; }
		constexpr T bottom() const { return y1; }
		constexpr T width() const { return x1 - x0; }
		constexpr T height() const { return y1 - y0; }
		constexpr T area() const { return width() * height(); }

	private:
		T x0, y0, x1, y1;
	};

	using Rectangle2D = RectangleArea<float>;

	/*Converts pixel size coordinates into device coordinates.
	  - coords: pixel size coordinates.
	  - w: screen width.
	  - h: screen height.
	  Result is a Rectangle2D with coordinates in range [-1,+1]. */
	//Rectangle2D coords2device(const Rectangle2D& coords, float w, float h);

	/*Converts device coordinates into pixel size coordinates.
	  - coords: device coordinates.
	  - w: screen width
	  - h: screen height
	  Result is a Rectangle2D with coordinates in range [0,w) and [0,h) respectively.*/
	//Rectangle2D device2coords(const Rectangle2D& dev, float w, float h);

	constexpr const float INVALID_DISTANCE = -1;

	struct Triangle
	{
		::std::array<::mpn::Point3, 3> vertices;

		constexpr Triangle(::std::array<::mpn::Point3, 3>&& vertices) : vertices(vertices) {};
		constexpr Triangle(::mpn::Point3 p0, ::mpn::Point3 p1, ::mpn::Point3 p2) : vertices{ p0, p1, p2 } {};
		
		float intersect(const geom::Line& line) const noexcept;

		::mpn::Point3 getCenter() const;
	};

	static_assert(sizeof(Triangle) <= 64, "Triangle not lightweight enough");

	struct AABB
	{
		::mpn::Point3 minCoords;
		::mpn::Point3 maxCoords;

		AABB() = default;
		AABB(::mpn::Point3 minCoords, ::mpn::Point3 maxCoords) : minCoords(minCoords), maxCoords(maxCoords)
		{
			if (!(minCoords[0] <= maxCoords[0] && minCoords[1] <= maxCoords[1] && minCoords[2] <= maxCoords[2]))
			{
				throw ::std::invalid_argument("Min coords must store the smaller coordinate for each dimensions");
			}
		}

		bool intersect(const geom::Line& line) const noexcept;
		inline ::mpn::Point3 getCenter() const noexcept {
			return ::mpn::Point3(
				(minCoords[0] + maxCoords[0]) / 2.0f,
				(minCoords[1] + maxCoords[1]) / 2.0f,
				(minCoords[2] + maxCoords[2]) / 2.0f
			);
		}

		bool contains(const AABB& other) const noexcept
		{
			return minCoords[0] <= other.minCoords[0]
				&& minCoords[1] <= other.minCoords[1]
				&& minCoords[2] <= other.minCoords[2]
				&& maxCoords[0] >= other.maxCoords[0]
				&& maxCoords[1] >= other.maxCoords[1]
				&& maxCoords[2] >= other.maxCoords[2];
		}

		static AABB _union(const AABB& left, const AABB& right);
	};

	AABB createAABB(const Triangle& triangle);
	AABB createAABB(std::vector<Triangle>::const_iterator begin, std::vector<Triangle>::const_iterator end);
}
