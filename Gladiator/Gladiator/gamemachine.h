#pragma once
#include "logs.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class GameMachine
{
	class State* current;

public:
	sf::RenderWindow window;
	GameMachine() {};
	
	void setCurrent(State* s) {

		current = s;
	}

	void setToGame();
	void setToMenu();
	void setToEditor();
	void run();
	
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

	//void setToGame();
	void run();
};

void GameMachine::run() {

	WelcomeScreenState welcomeState(this);

	welcomeState.run();

}

void WelcomeScreenState::run() {
	
	sf::RenderWindow window(sf::VideoMode(1900, 1000), "SFML works!!");
	sf::Texture texture;
	sf::Event event;
	sf::Sprite WelcomeScreen;
	sf::String nickName;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text playerText(nickName, font, 50);
	playerText.setPosition(450,550);
	playerText.setColor(sf::Color::White);

	if (!texture.loadFromFile("Images/welcome_screen.png"))
	{
		LOG_ERROR("Can't load the file!");
	}

	WelcomeScreen.setTexture(texture);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode >= 32 && event.text.unicode <= 126 && nickName.getSize() < 20)
					nickName += (char)event.text.unicode;
				else if (event.text.unicode == 8 && nickName.getSize() > 0)
					nickName.erase(nickName.getSize() - 1, nickName.getSize());
				playerText.setString(nickName);
			}
		}
		window.clear();
		window.draw(WelcomeScreen);
		window.draw(playerText);
		window.display();
	}
}


/*
class Game {
public:
	explicit Game(sf::RenderTarget &window);
};

class GameState : public State
{
public:

	Game game;

	GameState(GameMachine* gm) : State(gm), game(gm->window) {}

	void setToMenu();
	void prepareMap();
	void prepareHero();
	void run();
};

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