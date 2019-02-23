#pragma once

#include <SFML/Graphics.hpp>
#include "gamemachine.h"
#include "easywsclient.hpp"
#include <cassert>
#include <WinSock2.h>
#include "dataprotocol.h"
#include <queue>

using easywsclient::WebSocket;
using json = nlohmann::json;

class ResourceManager
{
public:
	sf::RenderWindow window;
	static std::queue<MessageData> messageInbox;
	static std::queue<MessageData> messageSendbox;
	float dx;
	float dy;

	ResourceManager();
};

class System
{
public:
	ResourceManager* resManager;

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void tearDown() = 0;
};

class ServerSystem : public System
{
public:

	ServerSystem(ResourceManager* rm);
	void init();
	void update();
	void tearDown();
};

class RenderSystem : public System
{
public:

	sf::View cameraView;
	sf::Texture playerTexture, mapTexture, swordTexture;
	sf::Sprite playerSprite, mapSprite, swordSprite;
	sf::RectangleShape healthbar, healthbarBackground;
	sf::Text health;
	sf::Font font;

	RenderSystem(ResourceManager* rm);
	void init();
	void update();
	void tearDown();

private:
	void lookAtMouse(sf::Sprite*);
	void updateHero();
};

class InputSystem : public System
{
public:

	sf::Event event;

	InputSystem(ResourceManager* rm);
	void init();
	void update();
	void tearDown();
};


class DataSystem : public System
{
public:

	int hp;

	DataSystem(ResourceManager* rm);
	void init();
	void update();
	void tearDown();
};