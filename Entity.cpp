#include "Entity.h"

Entity::Entity(int entityID, EntityType entityType)
	: m_ID(entityID),
	m_type(entityType)
{
}

EntityType Entity::getEntityType() const
{
	return m_type;
}

void Entity::render(sf::RenderWindow & window) const
{
	window.draw(m_sprite);
}
