#pragma once

#include "Engine/Log.h"


namespace Engine
{

	template<class T>
	class Grid
	{
	public:
		Grid(size_t width, size_t height)
			: m_Width(width), m_Height(height), m_Data(width * height)
		{
			ENG_CORE_ASSERT(width > 0 && height > 0, "Width and height must be greater than 0!");
		}

		size_t GetWidth() { return m_Width; }
		size_t GetHeight() { return m_Height; }

		inline T& operator() (size_t x, size_t y)
		{
			ENG_CORE_ASSERT_FMT(x < m_Width, "Invalid index x! value: {} allowed range: [0, {}]", x , m_Width - 1);
			ENG_CORE_ASSERT_FMT(y < m_Height, "Invalid index y! value: {} allowed range: [0, {}]", y, m_Height - 1);

			return m_Data[x * m_Width + y];
		}

		inline T operator() (size_t x, size_t y) const
		{
			ENG_CORE_ASSERT_FMT(x < m_Width, "Invalid index x! value: %i allowed range: [0, %i]", x, m_Width - 1);
			ENG_CORE_ASSERT_FMT(y < m_Height, "Invalid index y! value: %i allowed range: [0, %i]", y, m_Height - 1);
			return m_Data[x * m_Width + y];
		}

	private:
		size_t m_Width;
		size_t m_Height;
		std::vector<T> m_Data;
	};
}