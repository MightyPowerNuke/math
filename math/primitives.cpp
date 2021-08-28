#include <limits>

#include "math.h"

#include "primitives.h"

namespace geom {

	/*Rectangle2D device2coords(const Rectangle2D& dev, float w, float h)
	{
		return Rectangle2D((dev.left() + 1.0f)*float(w) / 2.0f,
			(dev.top() + 1.0f)*float(h) / 2.0f,
			(dev.right() + 1.0f)*float(w) / 2.0f,
			(dev.bottom() + 1.0f)*float(h) / 2.0f);
	}

	Rectangle2D coords2device(const Rectangle2D& coords, float w, float h)
	{
		return Rectangle2D(2.0f*coords.left() / w - 1.0f,
			2.0f*coords.top() / float(h) - 1.0f,
			2.0f*coords.right() / float(w) - 1.0f,
			2.0f*coords.bottom() / float(h) - 1.0f);
	}*/

	float Triangle::intersect(const geom::Line& line) const noexcept
    {
        // Möller-Trumbore intersection algorithm straight from Wikipedia
        const mpn::Vector3 edge1 = vertices[1] - vertices[0];
        const mpn::Vector3 edge2 = vertices[2] - vertices[0];
        const mpn::Vector3 h = line.v % edge2;
        const float a = edge1 * h;
        if (a > -mpn::EPSILON && a < mpn::EPSILON)
            return INVALID_DISTANCE;    // This ray is parallel to this triangle.
        const float f = 1.0f / a;
        const mpn::Vector3 s = line.P - vertices[0];
        const float u = f * (s * h);
        if (u < 0.0f || u > 1.0f)
            return INVALID_DISTANCE;
        const mpn::Vector3 q = s % edge1;
        const float v = f * line.v * q;
        if (v < 0.0f || u + v > 1.0f)
            return INVALID_DISTANCE;
        return f * (edge2 * q);
    }

    ::mpn::Point3 Triangle::getCenter() const
    {
        return ::mpn::Point3
        {
            (vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3.0f,
            (vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3.0f,
            (vertices[0][2] + vertices[1][2] + vertices[2][2]) / 3.0f,
        };
    }

    bool AABB::intersect(const geom::Line& line) const noexcept
    {
        // Fast branchless intersection test
        // stolen from https://tavianator.com/2011/ray_box.html
        // and https://tavianator.com/2015/ray_box_nan.html
        // Using the version that does not handle edge cases (i.e. corner, edge, and in plane rays all return false)
        // as this needs to be lightning fast. Any artifacts can be eliminated by storing primitives with epsilon larger boxes.

        // Can be sped up even further by passing inverses externally instead of calculating here.
        const float vInvX = 1.0f / line.v[0];
        const float vInvY = 1.0f / line.v[1];
        const float vInvZ = 1.0f / line.v[2];
        float tx1 = (minCoords[0] - line.P[0]) * vInvX;
        float tx2 = (maxCoords[0] - line.P[0]) * vInvX;

        float tmin = std::min(tx1, tx2);
        float tmax = std::max(tx1, tx2);

        float ty1 = (minCoords[1] - line.P[1]) * vInvY;
        float ty2 = (maxCoords[1] - line.P[1]) * vInvY;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        float tz1 = (minCoords[2] - line.P[2]) * vInvZ;
        float tz2 = (maxCoords[2] - line.P[2]) * vInvZ;

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));

        return tmax >= std::max(tmin, 0.0f);
    }

    AABB AABB::_union(const AABB& left, const AABB& right)
    {
        return AABB
        {
            {
                std::min(left.minCoords[0], right.minCoords[0]),
                std::min(left.minCoords[1], right.minCoords[1]),
                std::min(left.minCoords[2], right.minCoords[2])
            },
            {
                std::max(left.maxCoords[0], right.maxCoords[0]),
                std::max(left.maxCoords[1], right.maxCoords[1]),
                std::max(left.maxCoords[2], right.maxCoords[2])
            }
        };
    }

    AABB createAABB(const Triangle& triangle)
    {
        return AABB
        {
            {
                std::min(triangle.vertices[0][0], std::min(triangle.vertices[1][0], triangle.vertices[2][0])),
                std::min(triangle.vertices[0][1], std::min(triangle.vertices[1][1], triangle.vertices[2][1])),
                std::min(triangle.vertices[0][2], std::min(triangle.vertices[1][2], triangle.vertices[2][2]))
            },
            {
                std::max(triangle.vertices[0][0], std::max(triangle.vertices[1][0], triangle.vertices[2][0])),
                std::max(triangle.vertices[0][1], std::max(triangle.vertices[1][1], triangle.vertices[2][1])),
                std::max(triangle.vertices[0][2], std::max(triangle.vertices[1][2], triangle.vertices[2][2]))
            }
        };
    }

    AABB createAABB(std::vector<Triangle>::const_iterator begin, std::vector<Triangle>::const_iterator end)
    {
        ::mpn::Point3 minCoords(FLT_MAX, FLT_MAX, FLT_MAX);
    	::mpn::Point3 maxCoords(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (std::vector<Triangle>::const_iterator it = begin; it != end; ++it)
        {
            for (int vertexID = 0; vertexID < 3; ++vertexID)
            {
                const ::mpn::Point3& vertex = it->vertices[vertexID];
                for (int axisID = 0; axisID < 3; ++axisID)
                {
                    if (vertex[axisID] < minCoords[axisID])
                    {
                        minCoords[axisID] = vertex[axisID];
                    }
                    if (vertex[axisID] > maxCoords[axisID])
                    {
                        maxCoords[axisID] = vertex[axisID];
                    }
                }
            }
        }
        return { minCoords, maxCoords };
    }

}