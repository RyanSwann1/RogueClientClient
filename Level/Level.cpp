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
	m_clientsOnServer()
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
		m_clientsOnServer.emplace_back(enemyInfo.m_position, enemyInfo.m_ID);
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

	for (const auto& enemy : m_clientsOnServer)
	{
		window.draw(enemy.m_sprite);
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
		updateClientfromServerPosition(serverMessage.m_clientID, serverMessage.m_position);
		break;
	case PacketType::Disconnect :
		removeClientFromServer(serverMessage.m_clientID);
		break;
	}
}

void Level::updateClientfromServerPosition(int clientID, sf::Vector2i newPosition)
{
	auto iter = std::find_if(m_clientsOnServer.begin(), m_clientsOnServer.end(), [clientID](const auto& player) { return clientID == player.m_ID; });
	assert(iter != m_clientsOnServer.end());
	iter->m_position = newPosition;
}

void Level::addClientFromServer(int clientID, sf::Vector2i startingPosition)
{
	auto cIter = std::find_if(m_clientsOnServer.cbegin(), m_clientsOnServer.cend(), [clientID](const auto& player) { return clientID == player.m_ID; });
	assert(cIter != m_clientsOnServer.cend());

	m_clientsOnServer.emplace_back(startingPosition, clientID);
}

void Level::removeClientFromServer(int clientID)
{
	auto cIter = std::find_if(m_clientsOnServer.cbegin(), m_clientsOnServer.cend(), [clientID](const auto& player) { return clientID == player.m_ID; });
	assert(cIter != m_clientsOnServer.cend());
	
	m_clientsOnServer.erase(cIter);
}