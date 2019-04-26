#pragma once

#include "GameState.h"
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <deque> 

enum class PacketType
{
	Connect = 0,
	Disconnect,
	HeartBeat,
	PlayerPosition,
	LatestGameState,
	NewProjectile
};

struct ServerMessage
{
	ServerMessage(int clientID, PacketType packetType, sf::Vector2i position = sf::Vector2i())
		: m_clientID(clientID),
		m_packetType(packetType),
		m_position(position)
	{}

	int m_clientID;
	PacketType m_packetType;
	sf::Vector2i m_position;
};

class Client
{
public:
	Client(sf::IpAddress serverIPAddress, unsigned short serverPortNumber);
	Client(Client&) = delete;
	Client& operator=(Client&) = delete;
	Client(Client&&) = delete;
	Client&& operator=(Client&&) = delete;
	~Client();

	std::deque<ServerMessage>& getMessageQueue();

	bool isConnected() const;
	bool receivedLatestGameData(GameState& gameState);

	bool connectToServer();
	void disconnect();
	
private:
	std::deque<ServerMessage> m_messageQueue;
	sf::IpAddress m_serverIPAddress;
	sf::TcpSocket m_tcpSocket;
	sf::UdpSocket m_udpSocket;
	std::thread m_listenTCPThread;
	std::thread m_listenUDPThread;
	unsigned short m_serverPortNumber;
	bool m_connected;
	int m_clientID;

	void listenForUDPMessages();
	void listenForTCPMessages();
};