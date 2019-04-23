#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SFML/Graphics.hpp" 
#include "../Player.h"
#include "../LevelDetails.h"

struct GameState;
class Level
{
public:
	Level(const LevelDetails& levelDetails);

	const LevelDetails& getDetails() const;
	std::unique_ptr<Player>& getPlayer();

	void setGameState(const GameState& latestGameState);

	void draw(sf::RenderWindow& window) const;
	void update(float deltaTime);

	void updateEnemyPosition(int clientID, sf::Vector2i newPosition);
	void addEnemy(int clientID, sf::Vector2i startingPosition);
	void removeEnemy(int clientID);

private:
	LevelDetails m_details;
	std::unique_ptr<Player> m_player;
	std::vector<std::unique_ptr<Entity>> m_enemies;
};