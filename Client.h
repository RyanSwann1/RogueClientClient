#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <deque> 

enum class PacketType
{
	Connect = 0,
	Disconnect,
	HeartBeat,
	PlayerPosition
};

struct ServerMessage
{
	ServerMessage(int clientID, PacketType packetType, sf::Vector2f position = sf::Vector2f())
		: m_clientID(clientID),
		m_packetType(packetType),
		m_position(position)
	{}

	int m_clientID;
	PacketType m_packetType;
	sf::Vector2f m_position;
};

class Level;
class Client
{
public:
	Client(sf::IpAddress serverIPAddress, unsigned short serverPortNumber, Level& level);
	Client(Client&) = delete;
	Client& operator=(Client&) = delete;
	Client(Client&&) = delete;
	Client&& operator=(Client&&) = delete;
	~Client();

	bool isConnected() const;

	bool connectToServer();
	void disconnect();
	

private:
	Level& m_level;
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