#include "Client.h"
#include <iostream>

constexpr int CONNECT_TIMEOUT = 5000; //Milliseconds
constexpr int RECEIVE_GAME_DATA_TIMEOUT = 5000; // Milliseconds
constexpr int INVALID_CLIENT_ID = -1;

Client::Client(sf::IpAddress serverIPAddress, unsigned short serverPortNumber)
	: m_serverIPAddress(serverIPAddress),
	m_tcpSocket(),
	m_udpSocket(),
	m_listenTCPThread(),
	m_listenUDPThread(),
	m_serverPortNumber(serverPortNumber),
	m_connected(false),
	m_clientID(INVALID_CLIENT_ID)
{}

Client::~Client()
{
	m_listenTCPThread.join();
	m_listenUDPThread.join();
}

bool Client::isConnected() const
{
	return m_connected;
}

bool Client::receivedGameState(GameState & gameState)
{
	sf::Clock timer;
	float elaspedTime = 0;
	m_tcpSocket.setBlocking(false);
	char data[100];
	size_t received;
	while (elaspedTime <= RECEIVE_GAME_DATA_TIMEOUT)
	{
		elaspedTime += timer.restart().asMilliseconds();
		if (m_tcpSocket.receive(data, 15000, received) != sf::Socket::Done)
		{
			continue;
		}


	}

	m_tcpSocket.setBlocking(true);
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

	while (serverConnectTimer.getElapsedTime().asMilliseconds() < CONNECT_TIMEOUT)
	{
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
			
		//Establish UDP connection
		if (m_udpSocket.bind(m_serverPortNumber, m_serverIPAddress) != sf::Socket::Done)
		{
			std::cout << "Couldn't bind client to port.\n";
			return false;
		}
			
		//Send back player starting position
			
		m_clientID = clientID;
		m_connected = true;
		m_udpSocket.setBlocking(true);
		std::cout << "Client Connected To Server\n";
		m_listenUDPThread = std::thread(&Client::listenForUDPMessages, this);
		m_listenTCPThread = std::thread(&Client::listenForTCPMessages, this);
		return true;
	}
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

		switch (static_cast<PacketType>(packetType))
		{
		case PacketType::PlayerPosition :
			
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
		case PacketType::CurrentGameState :
			
			break;
		}
	}
}

void Client::handleCurrentGameStateMessage(sf::Packet& packet)
{
	/*std::string currentLevelName;
	std::vector<Players> playersInPlay;*/

}