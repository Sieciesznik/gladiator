#include "systems.h"

const int GAME_WIDTH = 1900, GAME_HEIGHT = 1000;

ResourceManager::ResourceManager() : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Labyrinth game"), dx(0), dy(0) {}

//=========================================================
//					Server System
//=========================================================

static WebSocket::pointer client;

static void handleMessage(const std::string & message)
{
	printf(">>> %s\n", message.c_str());
	if (message == "world") { client->close(); }
}

ServerSystem::ServerSystem(ResourceManager* rm) 
{ 
	resManager = rm;
};

void ServerSystem::init()
{
	INT rc;
	WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		LOG_ERROR("WSAStartup Failed.\n");
	}

	client = WebSocket::from_url("ws://arena.ympek.net:8020");
}

void ServerSystem::update()
{
	//assert(client);
	//client->send("hello");
	if (client != NULL) 
	{
		client->poll();
		client->dispatch(handleMessage);
	}
}

void ServerSystem::tearDown()
{
	WSACleanup();
}

//=========================================================
//					Render System
//=========================================================

RenderSystem::RenderSystem(ResourceManager* rm)
{
	resManager = rm;
};

void RenderSystem::init() 
{
	
	if (!playerTexture.loadFromFile("Images/NoClass.png") || !mapTexture.loadFromFile("Images/floor.jpg"))
	{
		LOG_ERROR("Can't load the file!");
	}
	mapTexture.setRepeated(true);
	mapSprite.setTexture(mapTexture);
	mapSprite.setTextureRect(sf::IntRect(0, 0, GAME_WIDTH*10, GAME_HEIGHT*10));
	mapSprite.setPosition(-GAME_WIDTH * 2,-GAME_WIDTH * 2);

	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(GAME_WIDTH/2, GAME_HEIGHT/2);
	playerSprite.setOrigin(50, 50);
	
	cameraView.setCenter(playerSprite.getPosition());
	cameraView.setSize(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT));
}

void RenderSystem::update()
{
	lookAtMouse();
	updateHero();
	resManager->window.clear();
	resManager->window.draw(mapSprite);
	resManager->window.draw(playerSprite);
	resManager->window.display();
}

void RenderSystem::tearDown()
{

}

void RenderSystem::lookAtMouse()
{
	sf::Vector2f spritePos = playerSprite.getPosition();
	sf::Vector2f mousePos = resManager->window.mapPixelToCoords(sf::Mouse::getPosition(resManager->window)) + sf::Vector2f(50, 50);

	// now we have both the sprite position and the cursor
	// position lets do the calculation so our sprite will
	// face the position of the mouse
	const float PI = 3.14159265;

	float dx = spritePos.x - mousePos.x;
	float dy = spritePos.y - mousePos.y;

	float rotation = (atan2(dy, dx)) * 180 / PI;
	playerSprite.setRotation(rotation);
}

void RenderSystem::updateHero()
{
	cameraView.setCenter(playerSprite.getPosition());
	resManager->window.setView(cameraView);
	playerSprite.move(resManager->dx, resManager->dy);
	
}

//=========================================================
//					Input System
//=========================================================

InputSystem::InputSystem(ResourceManager* rm)
{
	resManager = rm;
};

void InputSystem::init()
{

}

void InputSystem::update()
{
	while (resManager->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			resManager->window.close();
			exit(0);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) resManager->dy = -10;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::W)) resManager->dy = 10;
	else resManager->dy = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) resManager->dx = -10;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A)) resManager->dx = 10;
	else resManager->dx = 0;
}

void InputSystem::tearDown()
{

}
