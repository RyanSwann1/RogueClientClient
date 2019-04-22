#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SFML/Graphics.hpp" 
#include "../Player.h"

struct LevelDetails
{
public:
	LevelDetails(sf::Vector2i size, int tileSize)
		: m_size(size),
		m_tileSize(tileSize)
	{}

	const sf::Vector2i m_size;
	const int m_tileSize;
};

struct TileSheet
{
	TileSheet(const std::string& name, int tileSize, int columns);

	sf::IntRect getTileLocation(int tileID, int tileSize) const;

	const int m_columns;
	sf::Texture m_texture;
};

class TileLayer
{
public:
	TileLayer(const std::vector<std::vector<int>>& tileLayer, const std::string& tileSheetName);

	const std::vector<std::vector<int>>& getTileLayer() const;
	const std::string& getName() const;
	void draw(const TileSheet& tileSheet, sf::RenderWindow& window, const LevelDetails& levelDetails) const;

private:
	const std::string m_name;
	const std::vector<std::vector<int>> m_tileLayer;
};

struct GameState;
class Level
{
public:
	Level(const LevelDetails& levelDetails, const std::vector<TileLayer>& tileLayer, std::vector<sf::Vector2i>&& collisionLayer);

	const LevelDetails& getDetails() const;
	const std::vector<TileLayer>& getTileLayer() const;
	const std::vector<sf::Vector2i>& getCollisionLayer() const;
	std::unique_ptr<Player>& getPlayer();

	//void setGameState(const )

	void draw(sf::RenderWindow& window, const TileSheet& tileSheet) const;
	void update(float deltaTime);

	void updatePlayerPosition(int clientID, sf::Vector2i newPosition);
	void addPlayer(int clientID, sf::Vector2i startingPosition);
	void removePlayer(int clientID);

private:
	const LevelDetails m_details;
	const std::vector<TileLayer> m_tileLayers;
	const std::vector<sf::Vector2i> m_collisionLayer;
	std::vector<std::unique_ptr<Player>> m_players;
};