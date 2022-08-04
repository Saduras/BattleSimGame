#pragma once

#include <vector>

namespace Engine
{
	/// <summary>
	/// Returns a uniform random number in the range 0.0f and 1.0f.
	/// </summary>
	/// <returns></returns>
	float GetRandomFloat();

	/// <summary>
	/// Returns a random integer in the (inclusive range) min and max.
	/// </summary>
	/// <param name="min">Minimal random value.</param>
	/// <param name="max">Maximum random value.</param>
	/// <returns></returns>
	int GetRandomInt(int min, int max);

	/// <summary>
	/// Returns a random int in the range 0, length - 1;
	/// </summary>
	/// <param name="length"></param>
	/// <returns></returns>
	int GetRandomIndex(size_t length);

	/// <summary>
	/// Returns a random entry from the given collection.
	/// </summary>
	/// <param name="collection">Collection to draw a random entry from.</param>
	/// <returns>Random entry.</returns>
	template<class T>
	T GetRandomEntry(std::vector<T> collection)
	{
		return collection[GetRandomIndex(collection.size())];
	}
}