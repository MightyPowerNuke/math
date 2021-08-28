#pragma once


#include "vector.h"
#include "spherical.h"
#include "point.h"
#include "polar.h"
#include "matrix.h"
#include "primitives.h"

namespace mpn {

	/*Transformation value struct. Basically an aggregation of a transformation matrix and its inverse.*/
	class Transform {
	public:
		Transform();
		explicit Transform(Point3 _position);
		explicit Transform(Vector3 _scaling);
		explicit Transform(Vector3 _scaling, Point3 _position);
		explicit Transform(Vector3 _scaling, Vector3 _translation);
		explicit Transform(Vector3 _scaling, geom::Line _rot_axis, float _rot_angle);
		explicit Transform(geom::Line _rot_axis, float _rot_angle);
		explicit Transform(geom::Line _rot_axis, float _rot_angle, Point3 _position);
		explicit Transform(Vector3 _scaling, geom::Line _rot_axis, float _rot_angle, Point3 _position);
		explicit Transform(Matrix4 trfMatrix, Matrix4 trfMatrixInverse);

		void translate(const Vector3& tr);
		void scale(const Vector3& sc);
		void rotate(const geom::Line& axis, float phi);

		Vector3 transform(const Vector3& vector) const;
		Point3 transform(const Point3& point) const;
		Point3 inverseTransform(const Point3& point) const;
		geom::Line inverseTransformLine(const geom::Line& line) const;

	private:
		friend Transform operator*(const Transform& lhs, const Transform& rhs);

		Matrix4 T, Tinv, Tinv_transpone;
	};

	Transform operator*(const Transform& lhs, const Transform& rhs);

	Matrix4 translationMatrix(const Vector3& tr);
	Matrix4 scalingMatrix(const Vector3& sc);
	Matrix4 rotationMatrix(const geom::Line& axis, float phi);

	extern const Transform IdentityTransform;

	template<typename T>
	SphericalVector<T> cartesianToSpherical(const Vector<3, T>& v)
	{
		const float radius = v.length();
		if (radius == 0) return SphericalVector<T>();

		const float theta = asinf(v[1] / radius);
		const float phi = atan2f(v[2], v[0]);
		return SphericalVector<T>(radius, theta, phi);
	}

	template<typename T>
	Vector<3, T> sphericalToCartesian(const SphericalVector<T>& v)
	{
		const float radius = v[0];
		const float theta = v[1];
		const float phi = v[2];

		const float partialResultForXAndZ = radius * cosf(theta);
		return Vector<3, T>(
			partialResultForXAndZ * cosf(phi),
			radius * sinf(theta),
			partialResultForXAndZ * sinf(phi));
	}

	template<typename T>
	PolarVector<T> cartesianToPolar(const Vector<2, T>& v)
	{
		const float radius = v.length();
		const float theta = atan2f(v[1], v[0]);
	    return PolarVector<T>(radius, theta);
	}

	template<typename T>
	Vector<2, T> polarToCartesian(const PolarVector<T>& v)
	{
		const float radius = v[0];
		const float theta = v[1];

		return Vector<2, T>(
			radius * cosf(theta),
			radius * sinf(theta));
	}
}