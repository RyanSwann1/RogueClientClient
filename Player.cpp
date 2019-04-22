#include "Player.h"
#include "PathFinding.h"

Player::Player(int entityID, EntityType entityType, sf::Vector2i startingPosition)
	: Entity(entityID, entityType, startingPosition),
	m_movementPoints(5),
	m_pathToTile(),
	m_movementTimer(1.5f)
{

}

void Player::update(float deltaTime)
{
	if (!m_pathToTile.empty())
	{
		m_movementTimer.update(deltaTime);
		if (m_movementTimer.isExpired())
		{
			m_currentPosition = m_pathToTile.front();
			m_pathToTile.pop_front();
			m_movementTimer.reset();
		}
	}
}

void Player::moveToPosition(sf::Vector2i newPosition, const Level& level)
{
	m_pathToTile = PathFinding::getPathToTile(m_currentPosition, newPosition, level, m_movementPoints);
}