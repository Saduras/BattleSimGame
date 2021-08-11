#pragma once

#include "CppUnitTest.h"

#include <glm/glm.hpp>

#include <spdlog/fmt/bundled/core.h>
#include <spdlog/fmt/bundled/format.h>

#include <string>


static const float epsilon = 0.0001f;

namespace AssertUtils {
	template<typename T>
	static void glmAreEqual(const T lhs, const T rhs) {
		std::string message = fmt::format("{0} != {1}", glm::to_string(lhs), glm::to_string(rhs));
		Assert::IsTrue(glm::all(glm::epsilonEqual(lhs, rhs, epsilon)), ToWString(message).c_str());
	}

	static std::wstring ToWString(std::string str) {
		return std::wstring(str.begin(), str.end());
	}
};