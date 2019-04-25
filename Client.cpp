#include "Client.h"
#include <iostream>

constexpr int CONNECT_TIMEOUT = 5000; //Milliseconds
constexpr int RECEIVE_GAME_DATA_TIMEOUT = 5000; // Milliseconds
constexpr int INVALID_CLIENT_ID = -1;

Client::Client(const sf::IpAddress& serverIPAddress, unsigned short serverPortNumber)
	: m_serverIPAddress(serverIPAddress),
	m_tcpSocket(),
	m_udpSocket(),
	m_listenTCPThread(),
	m_listenUDPThread(),
	m_serverPortNumber(serverPortNumber),
	m_connected(false),
	m_clientID(INVALID_CLIENT_ID)
{
	int  i = 0;
}

Client::~Client()
{
	m_listenTCPThread.join();
	m_listenUDPThread.join();
}

std::deque<ServerMessage>& Client::getMessageQueue()
{
	return m_messageQueue;
}

bool Client::isConnected() const
{
	return m_connected;
}

bool Client::receivedLatestGameData(GameState& latestGameState)
{
	sf::Clock timer;
	float elaspedTime = 0;
	m_tcpSocket.setBlocking(false);
	char data[150000];
	size_t received;
	while (elaspedTime <= RECEIVE_GAME_DATA_TIMEOUT)
	{
		elaspedTime += timer.restart().asMilliseconds();
		if (m_tcpSocket.receive(data, 150000, received) != sf::Socket::Done)
		{
			continue;
		}

		PacketType packetType = (PacketType)data[0];
		if (packetType == PacketType::LatestGameState)
		{
			std::string levelName = (std::string)&data[sizeof(packetType)];
			sf::Vector2i* playerStartingPosition = (sf::Vector2i*)&data[sizeof(packetType) + sizeof(levelName)];
			std::vector<sf::Vector2i>* enemyPositions = (std::vector<sf::Vector2i>*)&data[sizeof(packetType) + sizeof(levelName)];
			std::vector<int>* enemyIDs = (std::vector<int>*)&data[sizeof(packetType) + sizeof(levelName) + sizeof(enemyPositions)];

			if (enemyPositions->size() != enemyIDs->size())
			{
				continue;
			}

			latestGameState.m_levelName = levelName;
			latestGameState.m_playerStartingPosition = *playerStartingPosition;
			for (int i = 0; i < enemyPositions->size(); ++i)
			{
				//	ClientOnServerProperties(sf::Vector2i position, int ID)
				sf::Vector2i enemyStartingPosition = enemyPositions->at(i);
				int enemyID = enemyIDs->at(i);
				latestGameState.m_enemies.emplace_back(enemyStartingPosition, enemyID);
			}

			m_tcpSocket.setBlocking(true);
			std::cout << "Connected to server\n";
			return true;
		}
		else
		{
			m_tcpSocket.setBlocking(true);
			break;
		}
	}

	m_tcpSocket.setBlocking(true);
	return false;
}

bool Client::connectToServer()
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
	m_udpSocket.setBlocking(false);
	sf::Clock serverConnectTimer;
	serverConnectTimer.restart();
	sf::Clock timer;
	float elaspedTime = 0;
	while (!m_connected)
	{
		elaspedTime += timer.restart().asSeconds();
		if (m_tcpSocket.receive(packet) != sf::Socket::Done)
		{
			continue;
		}
			
		//Get Client ID from Server
		int packetType = 0;
		int clientID = INVALID_CLIENT_ID;
		packet >> packetType >> clientID;			
		if (static_cast<PacketType>(packetType) != PacketType::Connect)
		{
			continue;
		}

		m_clientID = clientID;
		m_connected = true;
		m_udpSocket.setBlocking(true);
		std::cout << "Client Connected To Server\n";
		m_listenUDPThread = std::thread(&Client::listenForUDPMessages, this);
		m_listenTCPThread = std::thread(&Client::listenForTCPMessages, this);
		return true;
	}

	return false;
}

void Client::disconnect()
{
	sf::Packet packet;
	packet << m_clientID << static_cast<int>(PacketType::Disconnect);
	if (m_tcpSocket.send(packet) != sf::Socket::Done)
	{
		std::cout << "Unable to send disconnection message\n";
	}
}

void Client::listenForUDPMessages()
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

void Client::listenForTCPMessages()
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
		packet << clientID << packetType;
		switch (static_cast<PacketType>(packetType))
		{
		case PacketType::Connect :
			//Handle new player connection
			break;
		case PacketType::Disconnect :
			//Handle player disconnection
			break;
		case PacketType::LatestGameState :
			break;
		}
	}
}