#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <utility>

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

struct LevelDetails
{
	LevelDetails(std::pair<int, sf::Vector2i> details, const std::vector<TileLayer>& tileLayers, const std::vector<TileSheet>& tileSheets,
		std::vector<sf::Vector2i>&& collisionLayer, int tileSize, sf::Vector2i levelSize)
		: m_levelSize(levelSize),
		m_tileSize(tileSize),
		m_tileLayers(tileLayers),
		m_collisionLayer(std::move(collisionLayer)),
		m_tileSheets(tileSheets)
	{}
	
	sf::Vector2i m_levelSize;
	int m_tileSize;
	std::vector<TileLayer> m_tileLayers;
	std::vector<sf::Vector2i> m_collisionLayer;
	std::vector<TileSheet> m_tileSheets;
};