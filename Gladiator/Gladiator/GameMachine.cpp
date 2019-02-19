#include <stdio.h>
#include "gamemachine.h"

GameMachine::GameMachine(ResourceManager* rm)
{
	resManager = rm;
	serverSys = new ServerSystem(rm);
	inputSys = new InputSystem(rm);
	renderSys = new RenderSystem(rm);
	dataSys = new DataSystem(rm);

	current = new WelcomeScreenState(this);
};


void GameMachine::setToGame() {
	current->setToGame();
}
void GameMachine::setToMenu() {
	current->setToMenu();
}
void GameMachine::setToEditor() {
	current->setToEditor();
}
void GameMachine::run() {
	current->run();
}
void GameMachine::initiateSystems() {
	serverSys->init();
	inputSys->init();
	renderSys->init();
}
void GameMachine::updateSystems() {
	serverSys->update();
	inputSys->update();
	renderSys->update();
	dataSys->update();
}