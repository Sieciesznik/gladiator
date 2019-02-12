#pragma once
#include "logs.h"
#include "systems.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class GameMachine
{
	class State* current;

public:

	class ResourceManager* resManager;
	class ServerSystem* serverSys;
	class RenderSystem* renderSys;
	class InputSystem* inputSys;
	GameMachine(ResourceManager* resManager);
	
	void setCurrent(State* s) {

		current = s;
	}

	void setToGame();
	void setToMenu();
	void setToEditor();
	void run();
	void initiateSystems();
	void updateSystems();
};

class State
{
protected:

	char nextState;
	bool isRunning;

public:

	GameMachine* gameMachine;

	State(GameMachine* gm) {
		gameMachine = gm;
		nextState = '0';
		isRunning = false;
	}

	virtual void setToWelcomeScreen() {
		LOG_DECISION(1, "Cannot change state to welcome screen more than once!");
	}

	virtual void setToGame() {
		LOG_DECISION(1, "Cannot change state, already in game!");
	}

	virtual void setToMenu() {
		LOG_DECISION(1, "Cannot change state, already in menu!");
	}

	virtual void setToEditor() {
		LOG_DECISION(1, "Cannot change state, already in editor!");
	}
	virtual void run() {
		LOG_ERROR("Can't run!");
	}

	void setTransition(char c) {
		nextState = c;
		isRunning = false;
	}

};

class WelcomeScreenState : public State
{
public:
	WelcomeScreenState(GameMachine* gm) : State(gm) {}

	void setToGame();
	void run();
};

class GameState : public State
{
public:

	GameState(GameMachine* gm) : State(gm) {}

	void setToMenu();
	void prepareMap();
	void prepareHero();
	void run();
};
/*
class MenuState : public State
{
public:
	MenuState(GameMachine* gm) : State(gm) {}

	void setToGame();
	void setToEditor();
	void run();
};

class Editor {
public:
	explicit Editor(sf::RenderTarget &window, GameMachine* gameMachine);
};

class EditorState : public State
{
public:

	Editor editor;

	EditorState(GameMachine* gm) : State(gm), editor(gm->window, gm) {}

	void setToMenu();
	void prepareMap();
	void saveMap();
	void run();
};
*/