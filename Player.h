#pragma once

#include "Entity.h"
#include "Timer.h"
#include <deque>

class Level;
class Player : public Entity
{
public:
	Player(int entityID, EntityType entityType, sf::Vector2i startingPosition);

	void update(float deltaTime) override final;
	void moveToPosition(sf::Vector2i newPosition, const Level& level);

private:
	int m_movementPoints;
	std::deque<sf::Vector2i> m_pathToTile;
	Timer m_movementTimer;
};