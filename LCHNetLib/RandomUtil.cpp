#include "pch.h"
#include "RandomUtil.h"

float RandomUtil::GetRandomFloat()
{
	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0.0, 1.0);
	auto generator = std::bind(dist, engine);
	return generator();
}

int32 RandomUtil::GetRandomRangeInt(int32 start, int32 end)
{
	std::mt19937 engine((uint32)time(NULL));
	std::uniform_int_distribution<int32> dist(start, end);
	auto generator = std::bind(dist, engine);
	return generator();
}
