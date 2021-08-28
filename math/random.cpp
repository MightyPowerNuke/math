#include "random.h"

#include <time.h>

namespace mpn
{
	thread_local std::default_random_engine randomEngine(time(nullptr));

	const std::uniform_real_distribution<float> realDistribution(-1.0f, 1.0f);
	const std::uniform_real_distribution<float> angleDistribution(0.0f, 360.0f);

	float frand()
	{
		return frand(0.0f, 1.0f);
	}

	float frand(float min, float max)
	{
		const std::uniform_real_distribution<float> distribution(min, max);
		return distribution(randomEngine);
	}

	int dice(int min, int max)
	{
		const std::uniform_int_distribution<int> distribution(min, max - 1);
		return distribution(randomEngine);
	}
}