#include "LevelDetails.h"

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

void TileLayer::draw(sf::RenderWindow& window, const LevelDetails& levelDetails) const
{
	for (int y = 0; y < levelDetails.m_levelSize.y; ++y)
	{
		for (int x = 0; x < levelDetails.m_levelSize.x; ++x)
		{
			const int tileID = m_tileLayer[y][x];
			if (tileID >= 0)
			{
				sf::Sprite tile(levelDetails.m_tileSheet.m_texture, levelDetails.m_tileSheet.getTileLocation(tileID, levelDetails.m_tileSize));
				tile.setPosition(sf::Vector2f(x * levelDetails.m_tileSize, y * levelDetails.m_tileSize));
				window.draw(tile);
			}
		}
	}
}

LevelDetails::LevelDetails()
{}

LevelDetails::LevelDetails(const std::vector<TileLayer>& tileLayers, const TileSheet& tileSheet, 
	std::vector<sf::Vector2i>&& collisionLayer, int tileSize, sf::Vector2i levelSize)
	: m_levelSize(levelSize),
	m_tileSize(tileSize),
	m_tileLayers(tileLayers),
	m_collisionLayer(std::move(collisionLayer)),
	m_tileSheet(tileSheet)
{}