#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>

enum class PacketType
{
	Connect = 0,
	Disconnect,
	PlayerPosition
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
	sf::IpAddress m_serverIPAddress;
	sf::TcpSocket m_tcpSocket;
	sf::UdpSocket m_udpSocket;
	std::thread m_listenTCPThread;
	std::thread m_listenUDPThread;
	unsigned short m_serverPortNumber;
	bool m_connected;
	int m_clientID;

	void listen();
	void listenForTCPMessages();
};