#pragma once

#include "GameState.h"
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <deque> 
#include <string>

enum class PacketType
{
	EstablishConnection = 0,
	RemoteClientConnection,
	Disconnect,
	ClientName,
	EnteredGame,
	HeartBeat,
	PlayerPosition,
	LatestGameData,
	LatestLevelName,
	LatestClientPositions
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

struct RemoteClientDetails
{
	int m_ID;
	std::string m_name;
};

class NetworkHandler
{
public:
	NetworkHandler(sf::IpAddress serverIPAddress, unsigned short serverPortNumber, std::string&& name);
	NetworkHandler(NetworkHandler&) = delete;
	NetworkHandler& operator=(NetworkHandler&) = delete;
	NetworkHandler(NetworkHandler&&) = delete;
	NetworkHandler&& operator=(NetworkHandler&&) = delete;
	~NetworkHandler();

	std::deque<ServerMessage>& getMessageQueue();

	bool isConnected() const;

	bool connectToServer();
	void disconnect();
	
private:
	const std::string m_name;
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