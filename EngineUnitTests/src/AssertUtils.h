#pragma once

#include "CppUnitTest.h"

#include <glm/glm.hpp>

#include <string>

#include "Engine/Math.h"
#include "Engine/String.h"


static const float epsilon = 0.0001f;

namespace AssertUtils {
	template<typename T>
	static void glmAreEqual(const T lhs, const T rhs) {
		std::string message = Engine::String::Format("{0} != {1}", glm::to_string(lhs), glm::to_string(rhs));
		Assert::IsTrue(glm::all(glm::epsilonEqual(lhs, rhs, epsilon)), ToWString(message).c_str());
	}

	static std::wstring ToWString(std::string str) {
		return std::wstring(str.begin(), str.end());
	}
};

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<std::vector<int>>(const class std::vector<int>& vec)
			{
				std::wstringstream ss;
				for (size_t i = 0; i < vec.size(); i++)
				{
					if (i != 0)
						ss << ", ";
					ss << vec[i];
				}
				return ss.str();
			}

			template<> static std::wstring ToString<Engine::Transform>(const Engine::Transform& transform)
			{
				std::wstringstream ss;
				ss << Engine::String::ToString(transform).c_str();
				return ss.str();
			}
		}
	}
}