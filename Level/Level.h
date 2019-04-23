#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SFML/Graphics.hpp" 
#include "../Player.h"

struct LevelDetails
{
	LevelDetails()
		: m_size(),
		m_tileSize(0)
	{}
	LevelDetails(sf::Vector2i size, int tileSize)
		: m_size(size),
		m_tileSize(tileSize)
	{}

	sf::Vector2i m_size;
	int m_tileSize;
};

struct TileSheet
{
	TileSheet();

	sf::IntRect getTileLocation(int tileID, int tileSize) const;
	void setTileSheet(const std::string& textureName, int columns);

	int m_columns;
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
	Level();

	const LevelDetails& getDetails() const;
	const std::vector<TileLayer>& getTileLayer() const;
	const std::vector<sf::Vector2i>& getCollisionLayer() const;
	std::unique_ptr<Player>& getPlayer();

	void setGameState(const GameState& latestGameState);

	void draw(sf::RenderWindow& window) const;
	void update(float deltaTime);

	void updateEnemyPosition(int clientID, sf::Vector2i newPosition);
	void addEnemy(int clientID, sf::Vector2i startingPosition);
	void removeEnemy(int clientID);

private:
	LevelDetails m_details;
	std::vector<TileLayer> m_tileLayers;
	std::vector<sf::Vector2i> m_collisionLayer;
	TileSheet m_tileSheet;
	std::unique_ptr<Player> m_player;
	std::vector<std::unique_ptr<Entity>> m_enemies;
};