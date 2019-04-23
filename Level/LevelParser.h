#pragma once

#include <string>

struct LevelDetails;
namespace LevelParser
{
	LevelDetails parseLevel(const std::string& levelName);
} 