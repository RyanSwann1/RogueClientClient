#include "PathFinding.h"
#include "Level/Level.h"
#include <algorithm>
#include <math.h>

bool isInBounds(const LevelDetails& levelDetails, int x, int y);
std::deque<sf::Vector2i> getNeighbours(sf::Vector2i source, const Level& level);
bool isTileCollidable(sf::Vector2i position, const std::vector<sf::Vector2i>& collisionLayer);
sf::Vector2i getNextPoint(sf::Vector2i destination, const Level& level, const std::deque<sf::Vector2i>& graph);
int getDifferenceBetweenPoints(sf::Vector2i tile, sf::Vector2i source);
bool isDestinationReached(sf::Vector2i currentPoint, sf::Vector2i destination);

bool isInBounds(const LevelDetails & levelDetails, int x, int y)
{
	return ((y >= 0 && y < levelDetails.m_levelSize.y) && (x >= 0 && x < levelDetails.m_levelSize.x));
}

int getDifferenceBetweenPoints(sf::Vector2i tile, sf::Vector2i source)
{
	return std::abs(source.x - tile.x) + std::abs(source.y - tile.y);
}

bool isDestinationReached(sf::Vector2i currentPoint, sf::Vector2i destination)
{
	return currentPoint == destination;
}

std::deque<sf::Vector2i> getNeighbours(sf::Vector2i source, const Level& level)
{
	std::deque<sf::Vector2i> neighbours;
	auto& levelDetails = level.getDetails();
	for (int y = 0; y <= 2; y += 2)
	{
		for (int x = 0; x <= 2; x += 2)
		{
			if (isInBounds(levelDetails, x, y) && !isTileCollidable({ x, y }, level.getDetails().m_collisionLayer))
			{
				neighbours.emplace_back(x, y);
			}
		}
	}

	return neighbours;
}

bool isTileCollidable(sf::Vector2i position, const std::vector<sf::Vector2i>& collisionLayer)
{
	auto cIter = std::find_if(collisionLayer.begin(), collisionLayer.end(), [position](const auto& tile) { return position == tile; });
	return (cIter != collisionLayer.cend());
}

sf::Vector2i getNextPoint(sf::Vector2i destination, const Level & level, const std::deque<sf::Vector2i>& graph)
{
	auto neighbours = getNeighbours(graph.back(), level);
	sf::Vector2i point;
	for (auto neighbour : neighbours)
	{
		int neighbourDifference = getDifferenceBetweenPoints(neighbour, destination);
		int sourceDifference = getDifferenceBetweenPoints(graph.back(), destination);
		if (neighbourDifference < sourceDifference)
		{
			point = neighbour;
		}
	}

	//assert(position);
	return point;
}

std::deque<sf::Vector2i> PathFinding::getPathToTile(sf::Vector2i source, sf::Vector2i destination, const Level& level, int movementPoints)
{
	if (movementPoints <= 0)
	{
		return std::deque<sf::Vector2i>();
	}

	std::deque<sf::Vector2i> graph;
	graph.push_back(source);
	bool generatePath = true;
	while (!generatePath)
	{
		graph.push_back(getNextPoint(destination, level, graph));
		
		--movementPoints;
		if (movementPoints <= 0 || isDestinationReached(graph.back(), destination))
		{
			generatePath = false;
		}
	}

	return graph;
}