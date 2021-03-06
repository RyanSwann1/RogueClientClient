#include "LevelParser.h"
#include "../Utilities/Base64.h"
#include "../Utilities/tinyxml.h"
#include "../LevelDetails.h"
#include "Level.h"
#include <vector>
#include <cassert>

std::vector<TileLayer> parseTileLayers(const TiXmlElement& rootElement, std::pair<int, sf::Vector2i> levelDetails);
std::pair<int, sf::Vector2i> parseLevelDetails(const TiXmlElement& rootElement);
std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, sf::Vector2i levelSize);
TileSheet parseTileSheet(const TiXmlElement& rootElement);
std::vector<sf::Vector2i> parseCollisionLayer(const TiXmlElement & rootElement, int tileSize);

LevelDetails LevelParser::parseLevel(const std::string& levelName)
{
	TiXmlDocument mapFile;
	bool mapLoaded = mapFile.LoadFile(levelName);
	//assert(mapLoaded);

	const auto& rootElement = mapFile.RootElement();
	auto levelDetails = parseLevelDetails(*rootElement);
	std::vector<TileLayer> tileLayers = parseTileLayers(*rootElement, levelDetails);
	std::vector<sf::Vector2i> collisionLayer = parseCollisionLayer(*rootElement, levelDetails.first);
	TileSheet tileSheet = parseTileSheet(*rootElement);

	return LevelDetails(tileLayers, tileSheet, std::move(collisionLayer), levelDetails.first, levelDetails.second);
}

std::vector<sf::Vector2i> parseCollisionLayer(const TiXmlElement & rootElement, int tileSize)
{
	std::vector<sf::Vector2i> collidablePositions;
	for (const auto* collisionLayerElement = rootElement.FirstChildElement(); collisionLayerElement != nullptr;
		collisionLayerElement = collisionLayerElement->NextSiblingElement())
	{
		if (collisionLayerElement->Value() != std::string("objectgroup"))
		{
			continue;
		}

		if (collisionLayerElement->Attribute("name") != std::string("Collision Layer"))
		{
			continue;
		}

		for (const auto* collisionElement = collisionLayerElement->FirstChildElement();
			collisionElement != nullptr; collisionElement = collisionElement->NextSiblingElement())
		{
			int xPosition = 0, yPosition = 0;
			collisionElement->Attribute("x", &xPosition);
			collisionElement->Attribute("y", &yPosition);
			//Hack for Tiled.
			yPosition -= tileSize;
			//collidablePositions.emplace_back(xPosition, yPosition, tileSize, tileSize);
		}
	}
	
	return collidablePositions;
}

std::vector<sf::Vector2i> parseEntityStartingPositions(const TiXmlElement & rootElement, int tileSize)
{
	std::vector<sf::Vector2i> entityStartingPositions;
	for (const auto* entityElementRoot = rootElement.FirstChildElement(); entityElementRoot != nullptr; entityElementRoot = entityElementRoot->NextSiblingElement())
	{
		if (entityElementRoot->Value() != std::string("objectgroup") || entityElementRoot->Attribute("name") != std::string("Entity Layer"))
		{
			continue;
		}

		for (const auto* entityElement = entityElementRoot->FirstChildElement(); entityElement != nullptr; entityElement = entityElement->NextSiblingElement())
		{
			sf::Vector2i startingPosition;
			entityElement->Attribute("x", &startingPosition.x);
			entityElement->Attribute("y", &startingPosition.y);
			startingPosition.y -= tileSize; //Tiled Hack

			entityStartingPositions.push_back(startingPosition);
		}
	}

	return entityStartingPositions;
}

TileSheet parseTileSheet(const TiXmlElement& rootElement)
{
	TileSheet tileSheet;
	for (const auto* tileSheetElement = rootElement.FirstChildElement();
		tileSheetElement != nullptr; tileSheetElement = tileSheetElement->NextSiblingElement())
	{
		if (tileSheetElement->Value() != std::string("tileset"))
		{
			continue;
		}

		std::string name = tileSheetElement->Attribute("name");
		std::string source =  tileSheetElement->FirstChildElement()->Attribute("source");
		sf::Vector2i size;
		int tileSize = 0;
		tileSheetElement->FirstChildElement()->Attribute("width", &size.x);
		tileSheetElement->FirstChildElement()->Attribute("height", &size.y);
		tileSheetElement->Attribute("tilewidth", &tileSize);
		int columns = size.x / tileSize;
		
		
		tileSheet.setTileSheet(name, columns);
		
	}

	return tileSheet;
}

std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, sf::Vector2i levelSize)
{
	std::vector<std::vector<int>> tileData;
	tileData.reserve(levelSize.y);

	std::string decodedIDs; //Base64 decoded information
	const TiXmlElement* dataNode = nullptr; //Store our node once we find it
	for (const TiXmlElement* e = tileLayerElement.FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			dataNode = e;
		}
	}
	assert(dataNode);

	Base64 base64;
	const TiXmlText* text = dataNode->FirstChild()->ToText();
	const std::string t = text->Value();
	decodedIDs = base64.base64_decode(t);

	const std::vector<int> layerColumns(levelSize.x);
	for (int i = 0; i < levelSize.y; ++i)
	{
		tileData.push_back(layerColumns);
	}

	for (int rows = 0; rows < levelSize.y; ++rows)
	{
		for (int cols = 0; cols < levelSize.x; ++cols)
		{
			tileData[rows][cols] = *((int*)decodedIDs.data() + rows * levelSize.x + cols) - 1;
		}
	}

	return tileData;
}

std::pair<int, sf::Vector2i> parseLevelDetails(const TiXmlElement& rootElement)
{
	sf::Vector2i levelSize;
	int tileSize = 0;
	rootElement.Attribute("width", &levelSize.x);
	rootElement.Attribute("height", &levelSize.y);
	rootElement.Attribute("tilewidth", &tileSize);
	std::pair<int, sf::Vector2i> levelDetails;
	levelDetails.first = tileSize;
	levelDetails.second = levelSize;

	return levelDetails;
}

std::vector<TileLayer> parseTileLayers(const TiXmlElement & rootElement, std::pair<int, sf::Vector2i> levelDetails)
{
	std::vector<TileLayer> tileLayers;
	for (const auto* tileLayerElement = rootElement.FirstChildElement(); 
		tileLayerElement != nullptr; tileLayerElement = tileLayerElement->NextSiblingElement())
	{
		if (tileLayerElement->Value() != std::string("layer"))
		{
			continue;
		}

		auto tileMap = decodeTileLayer(*tileLayerElement, levelDetails.second);
		std::string name = tileLayerElement->Attribute("name");
		tileLayers.emplace_back(std::move(name), std::move(tileMap));
	}

	assert(!tileLayers.empty());
	return tileLayers;
}