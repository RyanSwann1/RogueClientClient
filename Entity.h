#pragma once

#include <SFML/Graphics.hpp>

enum class EntityType
{
	Player = 0,
	Enemy,
	PickUpHealth
};

class NetworkHandler;
class Entity
{
public:
	Entity(int entityID, EntityType entityType);

	EntityType getEntityType() const;

	virtual void update(NetworkHandler& networkHandler) = 0;
	void render(sf::RenderWindow& window) const;

private:
	const int m_ID;
	const EntityType m_type;
	sf::Sprite m_sprite;
};