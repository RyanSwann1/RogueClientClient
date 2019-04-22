#pragma once

#include "SFML/Graphics.hpp"

class Selector
{
public:
	Selector(sf::RenderWindow& window, int tileSize);
	Selector(const Selector&) = delete;
	Selector& operator=(const Selector&) = delete;
	Selector(Selector&&) = delete;
	Selector&& operator=(Selector&&) = delete;

	sf::Vector2i getPosition();

	void update(sf::Vector2i levelSize);
	void render(sf::RenderWindow& window) const;

private:
	sf::RectangleShape m_mouseRect;
	sf::IntRect m_mouseAABB;
};