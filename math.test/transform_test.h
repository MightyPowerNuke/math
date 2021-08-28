#pragma once


#include "../nuketest/nuketest/use_nuketest.h"

#include "..\math\math.h"
#include "..\math\transform.h"
#include "..\math\point.h"

TEST_MODULE(Transform)
{
	TEST(PointIdentityTransformation)
	{
		mpn::Point3 point(-1, 2, 3);
		ASSERT_EQUALS(mpn::IdentityTransform.transform(point), point);
	}
	
	TEST(CartesianPolarTransformationIntegrity)
	{
		mpn::Vector2 vector(5.5f, 2.4f);
		ASSERT_EQUALS(mpn::polarToCartesian(mpn::cartesianToPolar(vector)), vector);
	}
	
	TEST(CartesianSphericalTransformationIntegrity)
	{
		for (int i = 0; i < 10; ++i)
		{
			const mpn::Vector3 vector(mpn::frand(), mpn::frand(), mpn::frand());
			ASSERT_EQUALS(mpn::sphericalToCartesian(mpn::cartesianToSpherical(vector)), vector);
		}
	}
	
	TEST(CoordinateSystemConvention_CartesianPolar)
	{
		ASSERT_EQUALS(mpn::cartesianToPolar(mpn::Vector2(1.0f, 0.0f)), mpn::PolarVector2(1.0f, 0 * PI_2));
		ASSERT_EQUALS(mpn::cartesianToPolar(mpn::Vector2(0.0f, 1.0f)), mpn::PolarVector2(1.0f, 1 * PI_2));
		ASSERT_EQUALS(mpn::cartesianToPolar(mpn::Vector2(-1.0f, 0.0f)), mpn::PolarVector2(1.0f, 2 * PI_2));
		ASSERT_EQUALS(mpn::cartesianToPolar(mpn::Vector2(0.0f, -1.0f)), mpn::PolarVector2(1.0f, 3 * PI_2));
	}
	
	TEST(CoordinateSystemConvention_CartesianSpherical_Axes)
	{
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(1.0f, 0.0f, 0.0f)), mpn::SphericalVector3(1.0f, 0.0f, 0.0f));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(-1.0f, 0.0f, 0.0f)), mpn::SphericalVector3(1.0f, 0.0f, PI));

		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(0.0f, 1.0f, 0.0f)), mpn::SphericalVector3(1.0f, PI_2, 0.0f));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(0.0f, -1.0f, 0.0f)), mpn::SphericalVector3(1.0f, -PI_2, 0.0f));

		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(0.0f, 0.0f, 1.0f)), mpn::SphericalVector3(1.0f, 0.0f, PI_2));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(0.0f, 0.0f, -1.0f)), mpn::SphericalVector3(1.0f, 0.0f, PI + PI_2));
	}
	
	TEST(CoordinateSystemConvention_CartesianSpherical_45DegreeAnglesInXZPlane)
	{
		const float sqrt2 = sqrtf(2.0f)/2.0f;
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(sqrt2, 0.0f, sqrt2)), mpn::SphericalVector3(1.0f, 0.0f, 1 * PI_4));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(-sqrt2, 0.0f, sqrt2)), mpn::SphericalVector3(1.0f, 0.0f, 3 * PI_4));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(-sqrt2, 0.0f, -sqrt2)), mpn::SphericalVector3(1.0f, 0.0f, 5 * PI_4));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(sqrt2, 0.0f, -sqrt2)), mpn::SphericalVector3(1.0f, 0.0f, 7 * PI_4));

	}
	
	TEST(CoordinateSystemConvention_CartesianSpherical_45DegreeAnglesInXYPlane)
	{
		const float sqrt2 = sqrtf(2.0f) / 2.0f;
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(sqrt2, sqrt2, 0.0f)), mpn::SphericalVector3(1.0f, +PI_4, 0.0f));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(-sqrt2, sqrt2, 0.0f)), mpn::SphericalVector3(1.0f, +PI_4, PI));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(-sqrt2, -sqrt2, 0.0f)), mpn::SphericalVector3(1.0f, -PI_4, PI));
		ASSERT_EQUALS(mpn::cartesianToSpherical(mpn::Vector3(sqrt2, -sqrt2, 0.0f)), mpn::SphericalVector3(1.0f, -PI_4, 0.0f));
	}
	
}
