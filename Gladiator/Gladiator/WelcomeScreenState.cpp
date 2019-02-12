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
		LOG_INFO(1, "Clicked, yay!");
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

	while (gameMachine->resManager->window.isOpen() && isRunning)
	{	

		while (gameMachine->resManager->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				nextState = '0';
				isRunning = false;
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
				if (isSpriteClicked(&PlayButton, &gameMachine->resManager->window)) 
				{
					nextState = 'p';
					isRunning = false;
				}
			}
		}
		gameMachine->resManager->window.clear();
		gameMachine->resManager->window.draw(WelcomeScreen);
		gameMachine->resManager->window.draw(PlayButton);
		gameMachine->resManager->window.draw(playerText);
		gameMachine->resManager->window.display();
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