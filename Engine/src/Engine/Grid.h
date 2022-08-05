#pragma once

#include "Engine/Log.h"


namespace Engine
{
	struct GridPoint
	{
		int x;
		int y;
	};


	template<class T>
	class Grid
	{
	public:
		Grid(int width, int height)
			: m_Width(width), m_Height(height), m_Data(width * height)
		{
			ENG_CORE_ASSERT(width > 0 && height > 0, "Width and height must be greater than 0!");
		}

		Grid(int width, int height, const T& initialValue)
			: m_Width(width), m_Height(height), m_Data(width* height, initialValue)
		{
			ENG_CORE_ASSERT(width > 0 && height > 0, "Width and height must be greater than 0!");
		}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		inline T& Get(int x, int y)
		{
			ENG_CORE_ASSERT_FMT(0 <= x && x < m_Width, "Invalid index x! value: {} allowed range: [0, {}]", x , m_Width - 1);
			ENG_CORE_ASSERT_FMT(0 <= y && y < m_Height, "Invalid index y! value: {} allowed range: [0, {}]", y, m_Height - 1);

			return m_Data[x * m_Height + y];
		}

		inline T Get(int x, int y) const
		{
			ENG_CORE_ASSERT_FMT(0 <= x && x < m_Width, "Invalid index x! value: %i allowed range: [0, %i]", x, m_Width - 1);
			ENG_CORE_ASSERT_FMT(0 <= y && y < m_Height, "Invalid index y! value: %i allowed range: [0, %i]", y, m_Height - 1);
			return m_Data[x * m_Height + y];
		}


		inline T& Get(GridPoint point) { return Get(point.x, point.y); }
		inline T& Get(GridPoint point) const { return Get(point.x, point.y); }

	private:
		int m_Width;
		int m_Height;
		std::vector<T> m_Data;
	};
}