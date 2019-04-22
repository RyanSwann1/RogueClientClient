#include "Client.h"
#include "Level/Level.h"
#include "Level/LevelParser.h"
#include "Selector.h"
#include <iostream>
#include <string>
#include <vector>

constexpr int INVALID_CLIENT_ID = -1;
constexpr int CONNECT_TIMEOUT = 5000; //Milliseconds

//struct Client
//{
//	Client(std::string ipAddress, unsigned short port)
//		: m_udpSocket(),
//		m_serverIP(ipAddress),
//		m_serverPortNumber(port),
//		m_clientID(INVALID_CLIENT_ID),
//		m_connected(false)
//	{
//
//	}
//	
//	bool connectToServer()
//	{
//		if (m_connected)
//		{
//			return true;
//		}
//
//		if (m_tcpSocket.connect(m_serverIP, m_serverPortNumber) != sf::Socket::Done)
//		{
//			std::cout << "Couldn't connect to server\n";
//			return false;
//		}
//		
//		sf::Packet packet;
//		m_udpSocket.setBlocking(false);
//		sf::Clock serverConnectTimer;
//		serverConnectTimer.restart();
//
//		//Wait until responce from server
//		while (serverConnectTimer.getElapsedTime().asMilliseconds() < CONNECT_TIMEOUT)
//		{
//			if (m_tcpSocket.receive(packet) != sf::Socket::Done)
//			{
//				continue;
//			}
//
//			//Get Client ID from Server
//			int packetType = 0;
//			int clientID = INVALID_CLIENT_ID;
//			packet >> packetType >> clientID;			
//			if (static_cast<PacketType>(packetType) != PacketType::Connect)
//			{
//				continue;
//			}
//
//			//Establish UDP connection
//			if (m_udpSocket.bind(m_serverPortNumber, m_serverIP) != sf::Socket::Done)
//			{
//				std::cout << "Couldn't bind client to port.\n";
//				return false;
//			}
//
//			//Send back player starting position
//
//			m_clientID = clientID;
//			m_connected = true;
//			m_udpSocket.setBlocking(true);
//			std::cout << "Client Connected To Server\n";
//			return true;
//		}
//		
//		std::cout << "Failed to connectToServer.\n";
//		m_udpSocket.setBlocking(true);
//		m_udpSocket.unbind();
//		m_tcpSocket.disconnect();
//		return false;
//	}
//
//	void disconnect()
//	{
//		if (!m_connected)
//		{
//			return;
//		}
//
//		sf::Packet packet;
//		packet << static_cast<int>(PacketType::Disconnect) << m_clientID;
//		if (m_tcpSocket.send(packet) == sf::Socket::Done)
//		{
//			std::cout << "Client disconnected from server.\n";
//		}
//
//		m_udpSocket.unbind();
//		m_tcpSocket.disconnect();
//		m_connected = false;
//	}
//
//	//
//	//in its own thread
//	//
//	void listen()
//	{
//		sf::Packet packet;
//		while (m_connected)
//		{
//			packet.clear();
//			if (m_udpSocket.receive(packet, m_serverIP, m_serverPortNumber) != sf::Socket::Done)
//			{
//				continue;
//			}
//
//			int packetType = 0;
//			int clientID = 0;
//			packet >> packetType >> clientID;
//			if (static_cast<PacketType>(packetType) == PacketType::Disconnect)
//			{
//				std::cout << "Removed: " + clientID << " from game.\n";
//
//			}
//		}
//	}
//
//	sf::UdpSocket m_udpSocket;
//	sf::TcpSocket m_tcpSocket;
//	sf::IpAddress m_serverIP;
//	unsigned short m_serverPortNumber;
//	int m_clientID; //Assigned by the server
//	bool m_connected;
//};

//TIle sheet to use for world
//https://opengameart.org/content/dungeon-tileset
//Now need a character pack

const std::string levelNames[6]
{
	"Level1",
	"Level2",
	"Level3",
	"Level4",
	"Level5",
	"Level6"
};

int main()
{
	Client client("121", 199);
	if (!client.connectToServer())
	{
		std::cout << "Failed to connectToServer.\n";
		std::cout << "Stopping application.\n";
		sf::sleep(sf::seconds(2.f));
		return;
	}

	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML_WINDOW", sf::Style::Default);
	
	//Level level(LevelParser::parseLevel(levelNames[0]));
	//Selector selector(window, level.getDetails().m_tileSize);

	//Receive Game Data Before Game Begins.
	//If not, don't start game - disconnect
	//Introduce time out feature to wait for current state of the map to be sent
	//if exceed time, server time out
	//Then server disconnects client

	sf::Event currentEvent;
	while (window.isOpen())
	{
		while (window.pollEvent(currentEvent))
		{
			switch (currentEvent.type)
			{
			case sf::Event::Closed :
				window.close();
				break;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			//level.getPlayer()->moveToPosition(selector.getPosition(), level);
		}

		//level.update(0.f);
		window.clear(sf::Color::Black);
		//level.draw(window);
	
	}

	//Disconnect Client From Server


	//Client client("192.168.0.14", 5030, level);
	//if (client.connectToServer())
	//{		
	//	while (client.isConnected())
	//	{
	//		char d;
	//		std::cin >> d;
	//		if (d == 'd')
	//		{
	//			client.disconnect();
	//		}
	//	}
	//}
	//else
	//{
	//	std::cout << "Failed to connectToServer.\n";
	//}
	      
	std::cout << "Stopping application.\n";
	sf::sleep(sf::seconds(2.f));
	
	return 0;
}

//std::thread t(&Client::listen, &client);
//t.join();

//struct MessageHandler
//{
//	MessageHandler(sf::TcpSocket& client)
//		: m_client(client)
//	{}
//
//	void receiveClientMessages()
//	{
//		while (true)
//		{
//			sf::Packet packet;
//			if (m_client.receive(packet) == sf::Socket::Done)
//			{
//				std::string message;
//				packet >> message;
//				std::cout << message << "\n";
//			}
//			//////////////////////m_client.set
//		}
//	}
//	
//	sf::TcpSocket& m_client;
//};
//
//struct Player
//{
//
//
//	sf::RectangleShape m_shape;
//};
//
//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML_WINDOW", sf::Style::Default);
//	window.setFramerateLimit(30);
//	sf::TcpSocket client;
//	if (client.connect("192.168.0.14", 5030) == sf::Socket::Done)
//	{
//		std::cout << "Client Connected to port\n";
//	}
//	else
//	{
//		std::cout << "Client failed to connect\n";
//	}
//
//	MessageHandler messageHandler(client);
//	std::thread t( &MessageHandler::receiveClientMessages, &messageHandler );
//	sf::Event sfmlEvent;
//	while (window.isOpen())
//	{
//		while (window.pollEvent(sfmlEvent))
//		{
//			sf::Packet packet;
//			if (sfmlEvent.type == sf::Event::KeyPressed)
//			{
//				if (sfmlEvent.key.code == sf::Keyboard::Key::A)
//				{
//					std::string message = "MoveLeft";
//					packet << message;
//					if (client.send(packet) != sf::Socket::Done)
//					{
//						std::cout << "Unable to send message to server\n";
//					}
//				}
//				else if (sfmlEvent.key.code == sf::Keyboard::Key::D)
//				{
//					std::string message = "MoveRight";
//					packet << message;
//					if (client.send(packet) != sf::Socket::Done)
//					{
//						std::cout << "Unable to send message to server\n";
//					}
//				}
//				else if (sfmlEvent.key.code == sf::Keyboard::Key::S)
//				{
//					std::string message = "MoveDown";
//					packet << message;
//					if (client.send(packet) != sf::Socket::Done)
//					{
//						std::cout << "Unable to send message to server\n";
//					}
//				}
//				else if (sfmlEvent.key.code == sf::Keyboard::Key::W)
//				{
//					std::string message = "MoveUp";
//					packet << message;
//					if (client.send(packet) != sf::Socket::Done)
//					{
//						std::cout << "Unable to send message to server\n";
//					}
//				}
//			}
//
//		}
//		
//		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
//		//{
//		//}
//		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
//		//{
//		//	std::string message = "MoveRight";
//		//	packet << message;
//		//	client.send(packet);
//		//}
//		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
//		//{
//		//	std::string message = "MoveUp\n";
//		//	packet << message;
//		//	client.send(packet);
//		//}
//		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
//		//{
//		//	std::string message = "MoveDown\n";
//		//	packet << message;
//		//	client.send(packet);
//		//}	
//	}
//
//	t.join();
//	return 0;
//}