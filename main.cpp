#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <string>

//packetType, clientID, 5
//packetType clientID, posX, posY

//Unable to unpack packet
//if (!(packet >> packetType))
//{
//	continue;
//}

constexpr int INVALID_CLIENT_ID = -1;
constexpr int CONNECT_TIMEOUT = 5000; //Milliseconds

enum class PacketType
{
	Connect = 0,
	Disconnect,
	PLAYER_NEW,
	PLAYER_UPDATE,
	PLAYER_MOVE,
	PLAYER_DAMAGE,
};

struct Client
{
	Client(std::string ipAddress, unsigned short port)
		: m_udpSocket(),
		m_serverIP(ipAddress),
		m_serverPortNumber(port),
		m_clientID(INVALID_CLIENT_ID),
		m_connected(false)
	{

	}
	
	bool connectToServer()
	{
		if (m_connected)
		{
			return true;
		}

		if (m_tcpSocket.connect(m_serverIP, m_serverPortNumber) != sf::Socket::Done)
		{
			std::cout << "Couldn't connect to server\n";
			return false;
		}
		
		sf::Packet packet;
		m_udpSocket.setBlocking(false);
		sf::Clock serverConnectTimer;
		serverConnectTimer.restart();

		//Wait until responce from server
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
			if (m_udpSocket.bind(m_serverPortNumber, m_serverIP) != sf::Socket::Done)
			{
				std::cout << "Couldn't bind client to port.\n";
				return false;
			}

			m_clientID = clientID;
			m_connected = true;
			m_udpSocket.setBlocking(true);
			std::cout << "Client Connected To Server\n";
			return true;
		}
		
		std::cout << "Failed to connectToServer.\n";
		m_udpSocket.setBlocking(true);
		m_udpSocket.unbind();
		m_tcpSocket.disconnect();
		return false;
	}

	void disconnect()
	{
		if (!m_connected)
		{
			return;
		}

		sf::Packet packet;
		packet << static_cast<int>(PacketType::Disconnect) << m_clientID;
		if (m_tcpSocket.send(packet) == sf::Socket::Done)
		{
			std::cout << "Client disconnected from server.\n";
		}

		m_udpSocket.unbind();
		m_tcpSocket.disconnect();
		m_connected = false;
	}

	//
	//in its own thread
	//
	void listen()
	{
		sf::Packet packet;
		while (m_connected)
		{
			packet.clear();
			if (m_udpSocket.receive(packet, m_serverIP, m_serverPortNumber) != sf::Socket::Done)
			{
				continue;
			}

			int packetType = 0;
			int clientID = 0;
			packet >> packetType >> clientID;
			if (static_cast<PacketType>(packetType) == PacketType::Disconnect)
			{
				std::cout << "Removed: " + clientID << " from game.\n";

			}
		}
	}

	sf::UdpSocket m_udpSocket;
	sf::TcpSocket m_tcpSocket;
	sf::IpAddress m_serverIP;
	unsigned short m_serverPortNumber;
	int m_clientID; //Assigned by the server
	bool m_connected;
};

struct Counter
{
	void increment()
	{
		++value;
	}

	int value = 0;
};

//int main()
//{
//	Counter counter; // contains one int
//
//	std::vector<std::thread> threads;
//	for (int i = 0; i < 5; ++i) {
//		threads.push_back(std::thread([&counter]() {
//			for (int i = 0; i < 100; ++i) {
//				counter.increment();
//			}
//		}));
//	}
//
//	for (auto& thread : threads) {
//		thread.join();
//	} // Threads are all finished.
//
//	std::cout << counter.value << std::endl;
//
//	char c;
//	std::cin >> c;
//
//
//	return 0;
//}

int main()
{
	Client client("192.168.0.14", 5030);
	if (client.connectToServer())
	{
		while (client.m_connected)
		{
			char d;
			std::cin >> d;
			if (d == 'd')
			{
				client.disconnect();
			}
		}
	}
	else
	{
		std::cout << "Failed to connectToServer.\n";
	}
	      
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