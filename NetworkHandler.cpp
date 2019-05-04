#include "NetworkHandler.h"
#include <iostream>

constexpr int CONNECT_TIMEOUT = 5; //Seconds
constexpr int RECEIVE_ALL_GAME_DATA = 5; //Seconds
constexpr int INVALID_CLIENT_ID = -1;

NetworkHandler::NetworkHandler(sf::IpAddress serverIPAddress, unsigned short serverPortNumber, std::string&& name)
	: m_name(std::move(name)),
	m_serverIPAddress(serverIPAddress),
	m_tcpSocket(),
	m_udpSocket(),
	m_listenTCPThread(),
	m_listenUDPThread(),
	m_serverPortNumber(serverPortNumber),
	m_connected(false),
	m_clientID(INVALID_CLIENT_ID)
{}

NetworkHandler::~NetworkHandler()
{
	m_listenTCPThread.join();
	m_listenUDPThread.join();
}

std::deque<ServerMessage>& NetworkHandler::getMessageQueue()
{
	return m_messageQueue;
}

bool NetworkHandler::isConnected() const
{
	return m_connected;
}

//bool Client::receivedLatestGameData(GameState& latestGameState)
//{
//	sf::Clock timer;
//	float elaspedTime = 0;
//	m_tcpSocket.setBlocking(false);
//	char data[150000];
//	size_t received;
//	while (elaspedTime <= RECEIVE_ALL_GAME_DATA)
//	{
//		elaspedTime += timer.restart().asSeconds();
//		if (m_tcpSocket.receive(data, 150000, received) != sf::Socket::Done)
//		{
//			continue;
//		}
//
//		PacketType packetType = (PacketType)data[0];
//		if (packetType == PacketType::LatestGameData)
//		{
//			std::string levelName = (std::string)&data[sizeof(packetType)];
//			sf::Vector2i* playerStartingPosition = (sf::Vector2i*)&data[sizeof(packetType) + sizeof(levelName)];
//			std::vector<sf::Vector2i>* enemyPositions = (std::vector<sf::Vector2i>*)&data[sizeof(packetType) + sizeof(levelName)];
//			std::vector<int>* enemyIDs = (std::vector<int>*)&data[sizeof(packetType) + sizeof(levelName) + sizeof(enemyPositions)];
//
//			if (enemyPositions->size() != enemyIDs->size())
//			{
//				continue;
//			}
//
//			latestGameState.m_levelName = levelName;
//			latestGameState.m_playerStartingPosition = *playerStartingPosition;
//			for (int i = 0; i < enemyPositions->size(); ++i)
//			{
//				//	ClientOnServerProperties(sf::Vector2i position, int ID)
//				sf::Vector2i enemyStartingPosition = enemyPositions->at(i);
//				int enemyID = enemyIDs->at(i);
//				latestGameState.m_enemies.emplace_back(enemyStartingPosition, enemyID);
//			}
//
//			m_tcpSocket.setBlocking(true);
//			std::cout << "Connected to server\n";
//			return true;
//		}
//		else
//		{
//			m_tcpSocket.setBlocking(true);
//			break;
//		}
//	}
//
//	m_tcpSocket.setBlocking(true);
//	return false;
//}

bool NetworkHandler::connectToServer()
{
	if (m_connected)
	{
		std::cout << "Allready connected to server\n";
		return true;
	}

	if (m_tcpSocket.connect(m_serverIPAddress, m_serverPortNumber) != sf::Socket::Done)
	{
		std::cout << "Couldn't connect to server\n";
		return false;
	}

	sf::Packet packet;
	m_tcpSocket.setBlocking(false);
	sf::Clock timer;
	float elaspedTime = 0;
	while (elaspedTime < CONNECT_TIMEOUT)
	{
		packet.clear();
		elaspedTime += timer.restart().asSeconds();
		if (m_tcpSocket.receive(packet) != sf::Socket::Done)
		{
			continue;
		}
		
		int packetType = 0;
		packet >> packetType;
		if (static_cast<PacketType>(packetType) != PacketType::EstablishConnection)
		{
			continue;
		}

		packet >> packetType >> m_clientID;
		break;
	}

	packet.clear();
	packet << static_cast<int>(PacketType::ClientName) << m_clientID;
	m_tcpSocket.send(packet);



	m_connected = true;
	m_tcpSocket.setBlocking(true);
	std::cout << "Client Connected To Server\n";
	m_listenUDPThread = std::thread(&NetworkHandler::listenForUDPMessages, this);
	m_listenTCPThread = std::thread(&NetworkHandler::listenForTCPMessages, this);
	return true;
}

void NetworkHandler::disconnect()
{
	sf::Packet packet;
	packet << m_clientID << static_cast<int>(PacketType::Disconnect);
	if (m_tcpSocket.send(packet) != sf::Socket::Done)
	{
		std::cout << "Unable to send disconnection message\n";
	}
}

void NetworkHandler::listenForUDPMessages()
{
	while (m_connected)
	{
		sf::Packet packet;
		if (m_udpSocket.receive(packet, m_serverIPAddress, m_serverPortNumber) != sf::Socket::Done)
		{
			continue;
		}

		int clientID = INVALID_CLIENT_ID;
		int packetType = 0;
		packet >> clientID >> packetType;
		//Don't act on information based off of same client
		if (m_clientID == clientID)
		{
			continue;
		}

		sf::Vector2i newPosition;
		switch (static_cast<PacketType>(packetType))
		{
		case PacketType::PlayerPosition :
			packet >> clientID >> packetType >> newPosition.x >> newPosition.y;
			m_messageQueue.emplace_back(clientID, static_cast<PacketType>(packetType), newPosition);
			break;
		}
	}
}

void NetworkHandler::listenForTCPMessages()
{
	while (m_connected)
	{
		sf::Packet packet;
		if (m_tcpSocket.receive(packet) != sf::Socket::Done)
		{
			continue;
		}

		int clientID = INVALID_CLIENT_ID;
		int packetType = 0;
		std::string clientName;
		packet << clientID << packetType;
		switch (static_cast<PacketType>(packetType))
		{
		case PacketType::RemoteClientConnection :
			packet >> packetType >> clientID >> clientName;

			break;
		case PacketType::Disconnect :

			break;
		}
	}
}