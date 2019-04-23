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

struct LevelDetails;
struct TileLayer
{
	TileLayer();
	TileLayer(std::string&& name, std::vector<std::vector<int>>&& tileLayer);

	void draw(sf::RenderWindow& window, const LevelDetails& levelDetails) const;

	std::string m_name;
	std::vector<std::vector<int>> m_tileLayer;
};

struct LevelDetails
{
	LevelDetails();
	LevelDetails(const std::vector<TileLayer>& tileLayers, const TileSheet& tileSheet,
		std::vector<sf::Vector2i>&& collisionLayer, int tileSize, sf::Vector2i levelSize);
	
	sf::Vector2i m_levelSize;
	int m_tileSize;
	std::vector<TileLayer> m_tileLayers;
	std::vector<sf::Vector2i> m_collisionLayer;
	TileSheet m_tileSheet;
};