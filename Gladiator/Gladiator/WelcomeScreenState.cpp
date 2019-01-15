#include <stdio.h>
#include "gamemachine.h"

/*
void WelcomeScreenState::setToGame() {

	gameMachine->setCurrent(new GameState(this->gameMachine));
	delete this;
}
*/

void WelcomeScreenState::run() {

	isRunning = true;

	sf::Time stopTime;
	sf::Time targetTime = sf::milliseconds(1000 / 60);
	sf::Clock clock;

	sf::Texture backgroundTexture;

	sf::Sprite backgroundSprite;


	if (!backgroundTexture.loadFromFile("wololo.jpg"))
	{
		LOG_ERROR("Loading menu image error! Forgot of something?");
		getchar();
		return;
	}
	backgroundSprite.setTexture(backgroundTexture);
	
	while (gameMachine->window.isOpen() && isRunning)
	{

		clock.restart();
		//===========================================================================================================================
		sf::Event event;

		while (gameMachine->window.pollEvent(event))
		{
			switch (event.type)
			{

			case sf::Event::Closed:
				gameMachine->window.close();
				std::exit(0);
				break;

			case sf::Event::KeyPressed:
				setTransition('p');
				break;

			default:
				break;
			}
		}


		gameMachine->window.draw(backgroundSprite);

		gameMachine->window.display();


		//===========================================================================================================================
		stopTime = clock.getElapsedTime();
		sf::sleep(targetTime - stopTime);
	}

	switch (nextState)
	{
	case '0':
		std::exit(0);
		break;

	case 'p':
		gameMachine->setToGame();
	default:
		break;
	}
}