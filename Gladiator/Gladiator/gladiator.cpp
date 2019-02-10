#include <iostream>
#include <stdio.h>
#include "logs.h"
#include "gamemachine.h"
#include <SFML/Graphics.hpp>

#include "easywsclient.hpp"
#include <cassert>
#include <WinSock2.h>

using namespace std;
using easywsclient::WebSocket;
static WebSocket::pointer client = NULL;


void handle_message(const std::string & message)
{
	printf(">>> %s\n", message.c_str());
	if (message == "world") { client->close(); }
}

int main() {

	LOG_INFO(1, "Startup is done!");
	
	GameMachine gameMachine;

	INT rc;
	WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
		return 1;
	}

	while (true) {

		client = WebSocket::from_url("ws://arena.ympek.net:8020");
		//assert(client);
		//client->send("hello");
		//client->poll();
		//client->dispatch(handle_message);
		LOG_INFO(1, "Websocket is connected!");

		gameMachine.run();
	}

	WSACleanup();

	return 0;
}
	/*
	{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Red);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
	}
	*/