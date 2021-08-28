#include "math.h"
#include <cassert>

namespace mpn {

	void solvequadric(float A, float B, float C, float& res1, float& res2) noexcept {
		if (A == 0)
		{
			res1 = NAN;
			res2 = NAN;
			return;
		}
		const float D = B * B - 4.0f * A * C;
		if (D > 0)
		{
			float sqrtD = sqrtf(D);
			res1 = (-B + sqrtD) / (2.0f * A);
			res2 = (-B - sqrtD) / (2.0f * A);
		}
		else if (D == 0)
		{
			res1 = -B / (2.0f * A);
			res2 = NAN;
		}
		else
		{
			res1 = NAN;
			res2 = NAN;
		}
	}

	void solvequartic(float A, float B, float C, float D, float E, float& res1, float& res2, float& res3, float& res4) noexcept {
		assert(A != 0);
		const float d0 = C * C - 3.0f * B * D + 12.0f * A * E;
		const float d1 = 2.0f * C * C * C - 9.0f * B * C * D + 27.0f * B * B * E + 27.0f * A * D * D - 72.0f * A * C * E;

		const float p = (8.0f * A * C - 3.0f * B * B) / (8.0f * A * A);
		const float q = (B * B * B - 4.0f * A * B * C + 8.0f * A * A * D) / (8.0f * A * A * A);

		const float Q = cbrtf((d1 + sqrtf(d1 * d1 - 4.0f * d0 * d0 * d0)) / 2.0f);
		const float S = 0.5f * sqrtf((-2.0f / 3.0f) * p + (1.0f / (3.0f * A)) * (Q + d0 / Q));

		const float sqrtvalp = 0.5f * sqrtf(-4.0f * S * S - 2.0f * p + q / S);
		const float sqrtvaln = 0.5f * sqrtf(-4.0f * S * S - 2.0f * p - q / S);

		res1 = -(B / (4.0f * A)) - S + sqrtvalp;
		res2 = -(B / (4.0f * A)) - S - sqrtvalp;
		res3 = -(B / (4.0f * A)) + S + sqrtvaln;
		res4 = -(B / (4.0f * A)) + S - sqrtvaln;
	}

	float smallerNonnegative(float a, float b) noexcept {
		if (a >= 0.0f && (a < b || b < 0.0f || isnan(b))) return a;
		if (b >= 0.0f && (b <= a || a < 0.0f || isnan(a))) return b;
		return -1.0f;
	}

}
