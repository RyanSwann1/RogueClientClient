#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SFML/Graphics.hpp" 
#include "../Player.h"

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