#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SFML/Graphics.hpp" 
#include "../Player.h"
#include "../LevelDetails.h"
#include "../GameState.h"

struct ServerMessage;

class Level
{
public:
	Level();
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;
	Level(Level&&) = delete;
	Level&& operator=(Level&&) = delete;

	const LevelDetails& getDetails() const;
	std::unique_ptr<Player>& getPlayer();

	void setGameState(const GameState& latestGameState);
	void draw(sf::RenderWindow& window) const;
	void update(float deltaTime);
	void receiveServerMessage(const ServerMessage& serverMessage);

private:
	LevelDetails m_details;
	std::unique_ptr<Player> m_player;
	std::vector<ClientOnServerProperties> m_clientsOnServer;

	void updateClientfromServerPosition(int clientID, sf::Vector2i newPosition);
	void addClientFromServer(int clientID, sf::Vector2i startingPosition);
	void removeClientFromServer(int clientID);
};