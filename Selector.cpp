#include "Selector.h"

Selector::Selector(sf::RenderWindow& window, int tileSize)
	: m_mouseRect(),
	m_mouseAABB(sf::Vector2i(sf::Mouse::getPosition(window).x - tileSize, sf::Mouse::getPosition(window).y - tileSize), sf::Vector2i(tileSize, tileSize))
{
	m_mouseRect.setSize(sf::Vector2f(tileSize, tileSize));
	m_mouseRect.setOutlineColor(sf::Color::Red);
	m_mouseRect.setOutlineThickness(2.5f);
	m_mouseRect.setFillColor(sf::Color::Transparent);
}

sf::Vector2i Selector::getPosition()
{
	return sf::Vector2i(m_mouseRect.getPosition().x, m_mouseRect.getPosition().y);
}

void Selector::update(sf::Vector2i levelSize)
{
	for (int y = 0; y < levelSize.y; ++y)
	{
		for (int x = 0; x < levelSize.x; ++x)
		{
			sf::IntRect tileRect(sf::Vector2i(x * 64, y * 64), sf::Vector2i(64, 64));
			if (m_mouseAABB.intersects(tileRect))
			{
				m_mouseRect.setPosition(tileRect.left, tileRect.top);
			}
		}
	}
}

void Selector::render(sf::RenderWindow & window) const
{
	window.draw(m_mouseRect);
}