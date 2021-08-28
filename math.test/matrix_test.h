#pragma once

#include "../nuketest/nuketest/use_nuketest.h"
#include "../math/vector.h"
#include "../math/matrix.h"

#pragma warning(disable: 26496) //just pollutes these short functions

TEST_MODULE(Matrix)
{
	TEST(MatrixVectorMultiplication_Identity)
	{
		mpn::Matrix4 matrix;
		mpn::loadIdentity(matrix);
		mpn::Vector3 vector(1, 2, 3);
		ASSERT_EQUALS(vector*matrix, vector);
	}
	TEST(MatrixVectorMultiplication_General)
	{
		mpn::Matrix4 matrix(
			4, 0, 0, 0,
			0, 4, 0, 0,
			0, 0, 4, 0,
			8, -24, 16, 1);
		mpn::Vector3 vector(1, 2, 3);
		ASSERT_EQUALS(vector*matrix, mpn::Vector3(4, 8, 12));
	}	
	TEST(MatrixPointMultiplication_Identity)
	{
		mpn::Matrix4 matrix;
		mpn::loadIdentity(matrix);
		mpn::Point3 point(1, 2, 3);
		ASSERT_EQUALS(point * matrix, point);
	}
	TEST(MatrixPointMultiplication_General)
	{
		mpn::Matrix4 matrix(
			4, 0, 0, 0,
			0, 4, 0, 0,
			0, 0, 4, 0,
			8, -24, 16, 1);
		mpn::Point3 point(-1, -1, -1);
		ASSERT_EQUALS(point * matrix, mpn::Point3(4, -28, 12));
	}
	TEST(MatrixMatrixMultiplication_Identity)
	{
		mpn::Matrix4 m1(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);
		mpn::Matrix4 m2;
		mpn::loadIdentity(m2);
		ASSERT_EQUALS(m1*m2, m1);
	}
	TEST(MatrixMatrixMultiplication_General)
	{
		mpn::Matrix4 m1(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);
		mpn::Matrix4 m2(
			0, 2, 0, 3,
			2, 6, 4, 8,
			9, 7, 5, 1,
			1, 1, 1, 0);
		ASSERT_EQUALS(m1*m2, mpn::Matrix4(
			35, 39, 27, 22,
			83, 103, 67, 70,
			131, 167, 107, 118,
			179, 231, 147, 166
		));
	}	
}
