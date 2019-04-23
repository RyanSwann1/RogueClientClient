#include "Level.h"
#include "../GameState.h"
#include "LevelParser.h"
#include <utility>
#include <assert.h>
#include <algorithm>

//TileSheet
TileSheet::TileSheet()
	: m_texture(),
	m_columns()
{}

sf::IntRect TileSheet::getTileLocation(int tileID, int tileSize) const
{
	return sf::IntRect((tileID % m_columns) * tileSize,
		(tileID / m_columns) * tileSize, tileSize, tileSize);
}

void TileSheet::setTileSheet(const std::string & textureName, int columns)
{
	m_texture.loadFromFile(textureName);
	m_columns = columns;
}

//TileLayer
TileLayer::TileLayer(const std::vector<std::vector<int>>& tileLayer, const std::string& tileSheetName)
	: m_name(tileSheetName),
	m_tileLayer(tileLayer)
{}

const std::vector<std::vector<int>>& TileLayer::getTileLayer() const
{
	return m_tileLayer;
}

const std::string & TileLayer::getName() const
{
	return m_name;
}

void TileLayer::draw(const TileSheet& tileSheet, sf::RenderWindow& window, const LevelDetails& levelDetails) const
{
	for (int y = 0; y < levelDetails.m_size.y; ++y)
	{
		for (int x = 0; x < levelDetails.m_size.x; ++x)
		{
			const int tileID = m_tileLayer[y][x];
			if (tileID >= 0)
			{
				sf::Sprite tile(tileSheet.m_texture, tileSheet.getTileLocation(tileID, levelDetails.m_tileSize));
				tile.setPosition(sf::Vector2f(x * levelDetails.m_tileSize, y * levelDetails.m_tileSize));
				window.draw(tile);
			}
		}
	}
}

//Level
Level::Level()
	: m_details(),
	m_tileLayers(),
	m_collisionLayer(),
	m_tileSheet(),
	m_player(),
	m_enemies()
{}

const LevelDetails & Level::getDetails() const
{
	return m_details;
}

const std::vector<TileLayer>& Level::getTileLayer() const
{
	return m_tileLayers;
}

const std::vector<sf::Vector2i>& Level::getCollisionLayer() const
{
	assert(!m_collisionLayer.empty());
	return m_collisionLayer;
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

	 LevelParser::parseLevel(latestGameState.m_levelName);

	m_player = std::make_unique<Player>(0, EntityType::Player, latestGameState.m_playerStartingPosition);
}

void Level::draw(sf::RenderWindow& window) const
{
	for (const auto& tileLayer : m_tileLayers)
	{
		tileLayer.draw(m_tileSheet, window, m_details);
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