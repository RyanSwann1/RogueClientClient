#pragma once

#include <SFML/Graphics.hpp>

enum class EntityType
{
	Player = 0,
	Enemy,
	PickUpHealth
};

class Entity
{
public:
	Entity(int entityID, EntityType entityType, sf::Vector2i startingPosition);

	int getID() const;
	EntityType getEntityType() const;
	void setPosition(sf::Vector2i newPosition);

	virtual void update(float deltaTime) = 0;
	void render(sf::RenderWindow& window) const;

protected:
	sf::Vector2i m_currentPosition;

private:
	const int m_ID;
	const EntityType m_type;
	sf::Sprite m_sprite;
};