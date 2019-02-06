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

	sf::RenderWindow window(sf::VideoMode(1900, 1000), "SFML works!!");
	sf::Texture texture;
	sf::Event event;
	sf::Sprite WelcomeScreen;
	sf::String nickName;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text playerText(nickName, font, 50);
	playerText.setPosition(450, 550);
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