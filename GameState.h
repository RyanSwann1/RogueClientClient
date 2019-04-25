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
	sf::Sprite m_sprite;
};

struct GameState
{
	GameState() {}
	GameState(const std::string& levelName, const std::vector<ClientOnServerProperties>& enemies,
		sf::Vector2i playerStartingPosition) 
		: m_levelName(levelName),
		m_enemies(enemies),
		m_playerStartingPosition(playerStartingPosition)
	{}

	std::string m_levelName;
	std::vector<ClientOnServerProperties> m_enemies;
	sf::Vector2i m_playerStartingPosition;
	int m_clientID;
};