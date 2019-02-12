#include <stdio.h>
#include "gamemachine.h"
#include <math.h>


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

	
	while (gameMachine->resManager->window.isOpen() && isRunning)
	{
		gameMachine->updateSystems();
	}
}