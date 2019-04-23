#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <utility>

struct TileSheet
{
	TileSheet();
	//	tileSheets.emplace(name, TileSheet( source, tileSize, columns ));
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
	void draw(sf::RenderWindow& window, const LevelDetails& levelDetails) const;

private:
	const std::string m_name;
	const std::vector<std::vector<int>> m_tileLayer;
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