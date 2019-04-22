#pragma once

#include <deque>
#include "SFML/Graphics.hpp"

class Level;
namespace PathFinding
{
	std::deque<sf::Vector2i> getPathToTile(sf::Vector2i source, sf::Vector2i destination, const Level& level, int movementPoints);
}