#pragma once

#include "Engine/Core.h"
#include "Engine/Math.h"

#include <spdlog/fmt/fmt.h>
#include <string>

namespace Engine::String
{
	ALIAS_TEMPLATE_FUNCTION(FormatString, fmt::format);

	std::string ToString(const Transform& transform);
}