#include "transform.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace mpn {

	const Transform IdentityTransform(identityMatrix, identityMatrix);

	Transform::Transform() {
		loadIdentity(T);
		loadIdentity(Tinv);
		loadIdentity(Tinv_transpone);
	}		
	
	Transform::Transform(Point3 _position)
	{
		loadIdentity(T);
		loadIdentity(Tinv);
		translate(_position.asVector());
	}

	Transform::Transform(Vector3 _scaling)
	{
		loadIdentity(T);
		loadIdentity(Tinv);
		scale(_scaling);
	}


	Transform::Transform(Vector3 _scaling, Point3 _position)
	{
		loadIdentity(T);
		loadIdentity(Tinv);
		scale(_scaling);
		translate(_position.asVector());
	}

	Transform::Transform(Vector3 _scaling, Vector3 _translation)
	{
		loadIdentity(T);
		loadIdentity(Tinv);
		scale(_scaling);
		translate(_translation);
	}

	Transform::Transform(Vector3 _scaling, geom::Line _rot_axis, float _rot_angle)
	{
		loadIdentity(T);
		loadIdentity(Tinv);
		scale(_scaling); 
		if (_rot_axis.v.length() > 0.0f) //required line - without it, it will try to rotate with (0,0,0) axis, which will obviously fail
			rotate(_rot_axis, _rot_angle);
	}

	Transform::Transform(geom::Line _rot_axis, float _rot_angle)
	{
		loadIdentity(T);
		loadIdentity(Tinv);
		if (_rot_axis.v.length() > 0.0f) //required line - without it, it will try to rotate with (0,0,0) axis, which will obviously fail
			rotate(_rot_axis, _rot_angle);
	}

	Transform::Transform(geom::Line _rot_axis, float _rot_angle, Point3 _position)
	{
		loadIdentity(T);
		loadIdentity(Tinv);
		if (_rot_axis.v.length() > 0.0f) //required line - without it, it will try to rotate with (0,0,0) axis, which will obviously fail
			rotate(_rot_axis, _rot_angle);
		translate(_position.asVector());
	}

	Transform::Transform(Vector3 _scaling, geom::Line _rot_axis, float _rot_angle, Point3 _position) {
		loadIdentity(T);
		loadIdentity(Tinv);
		scale(_scaling);
		if (_rot_axis.v.length() > 0.0f) //required line - without it, it will try to rotate with (0,0,0) axis, which will obviously fail
			rotate(_rot_axis, _rot_angle);
		translate(_position.asVector());
	}

	Transform::Transform(Matrix4 trfMatrix, Matrix4 trfMatrixInverse) 
		: T(trfMatrix), Tinv(trfMatrixInverse), Tinv_transpone(Tinv.asTransposed())
	{
	}
	
	void Transform::translate(const Vector3& tr)
	{
		T = T * translationMatrix(tr);
		Tinv = translationMatrix(-tr) * Tinv;
		Tinv_transpone = Tinv.asTransposed();
	}

	void Transform::scale(const Vector3& sc)
	{
		T = T * scalingMatrix(sc);
		Tinv = scalingMatrix(Vector3(1.0f / sc[0], 1.0f / sc[1], 1.0f / sc[2])) * Tinv;
		Tinv_transpone = Tinv.asTransposed();
	}

	void Transform::rotate(const geom::Line& axis, float phi)
	{
		T = T * rotationMatrix(axis, phi);
		Tinv = rotationMatrix(axis, -phi) * Tinv;
		Tinv_transpone = Tinv.asTransposed();
	}

	Matrix4 translationMatrix(const Vector3& tr)
	{
		return Matrix4(
			1, 0, 0, 0, 
			0, 1, 0, 0, 
			0, 0, 1, 0, 
			tr[0], tr[1], tr[2], 1);
	}

	Matrix4 scalingMatrix(const Vector3& sc)
	{
		return Matrix4(
			sc[0], 0, 0, 0,
			0, sc[1], 0, 0,
			0, 0, sc[2], 0,
			0, 0, 0, 1.0f);
	}

	Matrix4 rotationMatrix(const geom::Line& axis, float phi)
	{
		if (axis.v.length() == 0.0f)
		{
			return identityMatrix;
		}
		const Vector3 normalizedAxisDirection = axis.v.asUnitVector();
		const float u = normalizedAxisDirection[0];
		const float v = normalizedAxisDirection[1];
		const float w = normalizedAxisDirection[2];
		const float a = axis.P[0];
		const float b = axis.P[1];
		const float c = axis.P[2];
		const float phir = phi * float(M_PI) / 180.0f;
		const float cosp = cosf(phir);
		const float sinp = sinf(phir);
		Matrix4 result;
		result(0, 0) = u*u + (v*v + w*w)*cosp;
		result(0, 1) = u*v*(1 - cosp) - w*sinp;
		result(0, 2) = u*w*(1 - cosp) + v*sinp;
		result(0, 3) = (a*(v*v + w*w) - u*(b*v + c*w))*(1 - cosp) + (b*w - c*v)*sinp;
		result(1, 0) = u*v*(1 - cosp) + w*sinp;
		result(1, 1) = v*v + (u*u + w*w)*cosp;
		result(1, 2) = v*w*(1 - cosp) - u*sinp;
		result(1, 3) = (b*(u*u + w*w) - v*(a*u + c*w))*(1 - cosp) + (c*u - a*w)*sinp;
		result(2, 0) = u*w*(1 - cosp) - v*sinp;
		result(2, 1) = v*w*(1 - cosp) + u*sinp;
		result(2, 2) = w*w + (u*u + v*v)*cosp;
		result(2, 3) = (c*(u*u + v*v) - w*(a*u + b*v))*(1 - cosp) + (a*v - b*u)*sinp;
		result(3, 3) = 1.0f;
		return result;
	}

	geom::Line Transform::inverseTransformLine(const geom::Line& line) const
	{
		/*Point3 gP(line.P * Tinv);
		return geom::Line(gP, (line.P + line.v) * Tinv - gP);*/
		return geom::Line(line.P * Tinv, line.v * Tinv); 
	}

	Vector3 Transform::transform(const Vector3& vector) const
	{
		return vector * Tinv_transpone;
	}

	Point3 Transform::transform(const Point3& point) const
	{
		return point * T;
	}
	
	Point3 Transform::inverseTransform(const Point3& point) const
	{
		return point * Tinv;
	}

	Transform operator*(const Transform& left, const Transform& right) 
	{
		return Transform(left.T * right.T, right.Tinv * left.Tinv);
	}
}