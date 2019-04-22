#include "Entity.h"

Entity::Entity(int entityID, EntityType entityType, sf::Vector2i startingPosition)
	: m_ID(entityID),
	m_type(entityType),
	m_currentPosition(startingPosition)
{}

EntityType Entity::getEntityType() const
{
	return m_type;
}

void Entity::render(sf::RenderWindow & window) const
{
	window.draw(m_sprite);
}