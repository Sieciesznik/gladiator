#include <stdio.h>
#include "gamemachine.h"
#include <math.h>


void GameState::setToMenu() {

	//gameMachine->setCurrent(new MenuState(this->gameMachine));
	//delete this;
}

void GameState::prepareMap() {
}


void GameState::prepareHero(sf::Texture *texture, sf::Sprite *sprite) {

	if (!texture->loadFromFile("Images/NoClass.png"))
	{
		LOG_ERROR("Can't load the file!");
	}

	sprite->setTexture(*texture);
	sprite->setPosition(600, 600);
	sprite->setOrigin(50, 50);
	
}

void lookAtMouse(sf::RenderWindow &window, sf::Sprite *sprite) {
	sf::Vector2f spritePos = sprite->getPosition();
	sf::Vector2i mousePos = sf::Mouse::getPosition(window) + sf::Vector2i(50, 50);

	// now we have both the sprite position and the cursor
	// position lets do the calculation so our sprite will
	// face the position of the mouse
	const float PI = 3.14159265;

	float dx = spritePos.x - mousePos.x;
	float dy = spritePos.y - mousePos.y;

	float rotation = (atan2(dy, dx)) * 180 / PI;
	sprite->setRotation(rotation);
}

void GameState::run() {

	isRunning = true;

	sf::Event event;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;

	this->prepareMap();
	this->prepareHero(&playerTexture, &playerSprite);

	
	while (gameMachine->window.isOpen())
	{
		
		while (gameMachine->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				gameMachine->window.close();
				exit(0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) playerSprite.move(-sqrt(2), -sqrt(2));
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) playerSprite.move(-sqrt(2), sqrt(2));
				else playerSprite.move(-1, 0);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) playerSprite.move(sqrt(2), -sqrt(2));
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) playerSprite.move(sqrt(2), sqrt(2));
				else playerSprite.move(1, 0);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) playerSprite.move(-sqrt(2), -sqrt(2));
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) playerSprite.move(sqrt(2), -sqrt(2));
				else playerSprite.move(0, -1);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) playerSprite.move(-sqrt(2), sqrt(2));
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) playerSprite.move(sqrt(2), sqrt(2));
				else playerSprite.move(0, 1);
			}
		}
	
	lookAtMouse(gameMachine->window, &playerSprite);

	gameMachine->window.clear();
	gameMachine->window.draw(playerSprite);
	gameMachine->window.display();
	}
}