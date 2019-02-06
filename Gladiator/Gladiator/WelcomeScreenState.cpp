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

	sf::Texture texture;
	sf::Event event;
	sf::Sprite WelcomeScreen;
	sf::String nickName;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text playerText(nickName, font, 50);
	playerText.setPosition(450, 550);
	playerText.setFillColor(sf::Color::White);

	if (!texture.loadFromFile("Images/welcome_screen.png"))
	{
		LOG_ERROR("Can't load the file!");
	}

	WelcomeScreen.setTexture(texture);

	while (gameMachine->window.isOpen())
	{
		while (gameMachine->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				gameMachine->window.close();
				exit(0);
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode >= 32 && event.text.unicode <= 126 && nickName.getSize() < 20)
					nickName += (char)event.text.unicode;
				else if (event.text.unicode == 8 && nickName.getSize() > 0)
					nickName.erase(nickName.getSize() - 1, nickName.getSize());
				playerText.setString(nickName);
			}
		}
		gameMachine->window.clear();
		gameMachine->window.draw(WelcomeScreen);
		gameMachine->window.draw(playerText);
		gameMachine->window.display();
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