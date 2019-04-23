#include "Level.h"
#include "../GameState.h"
#include "LevelParser.h"
#include <utility>
#include <assert.h>
#include <algorithm>
#include "../Client.h"

//Level
Level::Level()
	: m_details(),
	m_player(),
	m_enemies()
{}

const LevelDetails & Level::getDetails() const
{
	return m_details;
}

std::unique_ptr<Player> & Level::getPlayer()
{
	return m_player;
}

void Level::setGameState(const GameState & latestGameState)
{
	for (auto& enemyInfo : latestGameState.m_enemies)
	{
		m_enemies.emplace_back(enemyInfo.m_ID, EntityType::Enemy, enemyInfo.m_position);
	}

	m_details = LevelParser::parseLevel(latestGameState.m_levelName);
	m_player = std::make_unique<Player>(0, EntityType::Player, latestGameState.m_playerStartingPosition);
}

void Level::draw(sf::RenderWindow& window) const
{
	for (const auto& tileLayer : m_details.m_tileLayers)
	{
		tileLayer.draw(window, m_details);
	}

	m_player->render(window);

	for (const auto& enemy : m_enemies)
	{
		enemy->render(window);
	}
}

void Level::update(float deltaTime)
{
	m_player->update(deltaTime);
}

void Level::receiveServerMessage(const ServerMessage & serverMessage)
{
	switch (serverMessage.m_packetType)
	{
	case PacketType::PlayerPosition :
		break;
	case PacketType::Disconnect :
		break;
	}
}

void Level::updateEnemyPosition(int clientID, sf::Vector2i newPosition)
{
	auto iter = std::find_if(m_enemies.begin(), m_enemies.end(), [clientID](const auto& player) { return clientID == player->getID(); });
	assert(iter != m_enemies.end());
	
	(*iter)->setPosition(newPosition);
}

void Level::addEnemy(int clientID, sf::Vector2i startingPosition)
{
	auto cIter = std::find_if(m_enemies.cbegin(), m_enemies.cend(), [clientID](const auto& player) { return clientID == player->getID(); });
	assert(cIter != m_enemies.cend());

	m_enemies.push_back(std::make_unique<Player>(clientID, EntityType::Player, startingPosition));
}

void Level::removeEnemy(int clientID)
{
	auto cIter = std::find_if(m_enemies.cbegin(), m_enemies.cend(), [clientID](const auto& player) { return clientID == player->getID(); });
	assert(cIter != m_enemies.cend());
	
	m_enemies.erase(cIter);
}