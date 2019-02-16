#include <stdio.h>
#include "gamemachine.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


void GameState::setToMenu() {

	//gameMachine->setCurrent(new MenuState(this->gameMachine));
	//delete this;
}

void GameState::prepareMap() {

}

void GameState::prepareHero() {
}

void GameState::run() {

	isRunning = true;
	

	this->prepareMap();
	
	this->prepareHero();

	sf::Time stopTime;
	sf::Time targetTime = sf::milliseconds(1000 / 60);
	sf::Clock clock;
	
	while (gameMachine->resManager->window.isOpen() && isRunning)
	{
		clock.restart();

		gameMachine->updateSystems();

		stopTime = clock.getElapsedTime();
		sf::sleep(targetTime - stopTime);
	}
}