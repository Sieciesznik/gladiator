#include <iostream>
#include <stdio.h>
#include "logs.h"
#include "gamemachine.h"
#include "systems.h"
#include <SFML/Graphics.hpp>

#include "easywsclient.hpp"
#include <cassert>
#include <WinSock2.h>

using namespace std;

int main() {

	LOG_INFO(1, "Startup is done!");
	
	ResourceManager resManager;
	GameMachine gameMachine(&resManager);

	gameMachine.initiateSystems();

	while (true) {

		gameMachine.run();
	}

	return 0;
}