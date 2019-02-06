#include <stdio.h>
#include "gamemachine.h"


GameMachine::GameMachine() : window(sf::VideoMode(1900, 1000), "Labyrinth game") {

	current = new WelcomeScreenState(this);

	current->run();

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