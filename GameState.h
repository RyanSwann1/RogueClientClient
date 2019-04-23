#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct ClientOnServerProperties
{
	ClientOnServerProperties(sf::Vector2i position, int ID)
		: m_position(position),
		m_ID(ID)
	{}

	sf::Vector2i m_position;
	int m_ID;
};

struct GameState
{
	std::string m_levelName;
	std::vector<ClientOnServerProperties> m_enemies;
	sf::Vector2i m_playerStartingPosition;
};