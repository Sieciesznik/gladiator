#include <stdio.h>
#include "gamemachine.h"

void WelcomeScreenState::setToGame() {

	gameMachine->setCurrent(new GameState(this->gameMachine));
}

bool isSpriteClicked(sf::Sprite *spr, sf::RenderWindow *window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	if (spr->getGlobalBounds().contains(mousePosF))
	{
		std::cout << "Clicked, yay!" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

void WelcomeScreenState::run() {

	isRunning = true;

	sf::Texture texture, ButtonTexture;
	sf::Event event;
	sf::Sprite WelcomeScreen, PlayButton;
	sf::String nickName;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text playerText(nickName, font, 50);
	playerText.setPosition(450, 550);
	playerText.setFillColor(sf::Color::White);

	if (!texture.loadFromFile("Images/welcome_screen.png") || !ButtonTexture.loadFromFile("Images/PlayButton.png"))
	{
		LOG_ERROR("Can't load the file!");
	}

	WelcomeScreen.setTexture(texture);
	PlayButton.setTexture(ButtonTexture);
	PlayButton.setPosition(850, 700);

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
				if (event.text.unicode >= 32 && event.text.unicode <= 126 && nickName.getSize() < 25)
					nickName += (char)event.text.unicode;
				else if (event.text.unicode == 8 && nickName.getSize() > 0)
					nickName.erase(nickName.getSize() - 1, nickName.getSize());
				playerText.setString(nickName);
			}
			if(event.type == sf::Event::MouseButtonPressed)
			{
				if (isSpriteClicked(&PlayButton, &gameMachine->window)) 
				{
					WelcomeScreenState::setToGame();
					gameMachine->run();
				}
			}
		}
		gameMachine->window.clear();
		gameMachine->window.draw(WelcomeScreen);
		gameMachine->window.draw(PlayButton);
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