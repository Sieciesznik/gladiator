#pragma once

#include <SFML/Graphics.hpp>
#include "gamemachine.h"
#include "easywsclient.hpp"
#include <cassert>
#include <WinSock2.h>

using easywsclient::WebSocket;

class ResourceManager
{
public:
	sf::RenderWindow window;
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

	sf::Texture playerTexture;
	sf::Sprite playerSprite;

	RenderSystem(ResourceManager* rm);
	void init();
	void update();
	void tearDown();

private:
	void lookAtMouse();
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