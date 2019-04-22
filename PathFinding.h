#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

class Level;
namespace PathFinding
{
	std::vector<sf::Vector2i> getPathToTile(sf::Vector2i source, sf::Vector2i destination, const Level& map, int movementPoints);
}