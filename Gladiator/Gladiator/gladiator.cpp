#include <iostream>
#include <stdio.h>
#include "logs.h"
#include "gamemachine.h"
#include <SFML/Graphics.hpp>

using namespace std;

int main() {

	LOG_INFO(1, "Startup is done!");
	
	GameMachine gameMachine;
	
	//while (true) {
		gameMachine.run();
	//}
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