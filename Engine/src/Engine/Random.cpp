#include "epch.h"
#include "Random.h"

#include <cstdlib>

namespace Engine
{
	float GetRandomFloat()
	{
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	}

	int GetRandomInt(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	int GetRandomIndex(size_t length)
	{
		return rand() % length;
	}
}