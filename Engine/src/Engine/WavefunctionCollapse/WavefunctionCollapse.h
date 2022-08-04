#pragma once

#include "Engine/Core.h"
#include "Engine/Grid.h"

#include <unordered_map>
#include <set>
#include <vector>

namespace Engine::WFC
{
	using RuleSet = std::unordered_map<int, std::set<int>>;

	void PropagateCell(const std::vector<int>& source, std::vector<int>& target, const RuleSet& rules);
	void UpdateAdjecentCells(Grid<std::vector<int>>& options, int x, int y, const RuleSet& rules);

}