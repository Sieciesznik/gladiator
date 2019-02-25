#include "systems.h"
#include <fstream>
#include <sstream>

const int GAME_WIDTH = 1900, GAME_HEIGHT = 1000, NUM_OF_PLAYERS = 10;

bool swordPicked = 0;
int playerIds[NUM_OF_PLAYERS], health_value[NUM_OF_PLAYERS];
float enemy_dx[NUM_OF_PLAYERS], enemy_dy[NUM_OF_PLAYERS];
float enemy_target_dx[NUM_OF_PLAYERS], enemy_target_dy[NUM_OF_PLAYERS];
float playerSpeed[NUM_OF_PLAYERS];
bool playerGotUpdate[NUM_OF_PLAYERS];

ResourceManager::ResourceManager() : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Labyrinth game"), dx(0), dy(0) {}
std::queue<MessageData> ResourceManager::messageInbox;
std::queue<MessageData> ResourceManager::messageSendbox;

//=========================================================
//					Server System
//=========================================================

static WebSocket::pointer client;
static ProtocolDecoder decoder;
static ProtocolEncoder encoder;

static void handleMessage(const std::string & message)
{
	ResourceManager::messageInbox.push(decoder.decode(message.c_str()));
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

	std::ifstream protocolFile("arenaProtocol.json");

	if (protocolFile)
	{
		std::stringstream buffer;

		buffer << protocolFile.rdbuf();

		protocolFile.close();


		json protocol = json::parse(buffer);
		decoder.set(protocol["serverToClientMessage"]["messageTypes"], (uint8_t)protocol["messageIdSize"]);
		encoder.set(protocol["clientToServerMessage"]["messageTypes"], (uint8_t)protocol["messageIdSize"]); 
	}

}

void ServerSystem::update()
{
	//assert(client);
	//client->send("hello");
	if (client != NULL) 
	{
		client->poll();
		client->dispatch(handleMessage);
		
		if (ResourceManager::messageSendbox.size() > 0)
		{
			std::string msgToSend = encoder.encode(ResourceManager::messageSendbox.front());
			client->sendBinary(msgToSend);
			
			ResourceManager::messageSendbox.pop();
		}
	}

}
void RenderSystem::updateHealth() {

	healthbarBackground[resManager->our_player_id].setSize(sf::Vector2f(604, 54));
	healthbarBackground[resManager->our_player_id].setFillColor(sf::Color::Black);
	healthbar[resManager->our_player_id].setSize(sf::Vector2f(resManager->health * 3 , 50));
	healthbar[resManager->our_player_id].setFillColor(sf::Color::Green);

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
	resManager->health = 200;

	if (!playerTexture.loadFromFile("Images/NoClass.png") || !mapTexture.loadFromFile("Images/floor.jpg") || !swordTexture.loadFromFile("Images/sword.png") || !angry.loadFromFile("Images/angry.png") || !dead.loadFromFile("Images/dead.png"))
	{
		LOG_ERROR("Can't load the file!");
	}
	font.loadFromFile("arial.ttf");
	health.setFont(font);
	health.setStyle(sf::Text::Bold);
	health.setFillColor(sf::Color::Black);


	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		playerIds[i] = -1;
		enemy_dx[i] = -1;
		enemy_dy[i] = -1;
		enemy_target_dx[i] = -1;
		enemy_target_dy[i] = -1;
		health_value[i] = -1;
	}
	mapTexture.setRepeated(true);
	mapSprite.setTexture(mapTexture);
	mapSprite.setTextureRect(sf::IntRect(0, 0, GAME_WIDTH*10, GAME_HEIGHT*10));
	mapSprite.setPosition(-GAME_WIDTH * 2,-GAME_WIDTH * 2);

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		playerSprite[i].setTexture(angry);
		if (resManager->our_player_id == i) {
			playerSprite[i].setPosition(resManager->dx, resManager->dy);
		}
		playerSprite[i].setOrigin(50, 50);
	}
	swordSprite.setTexture(swordTexture);
	swordSprite.setPosition((GAME_WIDTH / 2)+30, (GAME_HEIGHT / 2)+30);
	swordSprite.setOrigin(84,84);
	
	cameraView.setSize(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT));
}

void RenderSystem::update()
{
	updateHero();
	updateHealth();
	lookAtMouse(&playerSprite[resManager->our_player_id]);
	//lookAtMouse(&swordSprite);
	resManager->window.clear();
	resManager->window.draw(mapSprite);
	resManager->window.draw(swordSprite);

	//std::cout << resManager->our_player_id << std::endl;

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		if (playerIds[i] != -1)
		{
			if (resManager->our_player_id == i)
			{	
				if (playerGotUpdate[i])
				{
					playerGotUpdate[i] = false;
						std::cout << resManager->target_x << "|" << resManager->dx << std::endl;
				}
				else
				{
					float vectX = resManager->target_x - resManager->dx;
					float vectY = resManager->target_y - resManager->dy;
					if (vectX != 0 || vectY != 0)
					{
						double vectLen = sqrt(vectX*vectX + vectY * vectY);
						vectX = (vectX / vectLen) * (playerSpeed[i] / 60);
						vectY = (vectY / vectLen) * (playerSpeed[i] / 60);
						resManager->dx += vectX;
						resManager->dy += vectY;
					}
				}
				playerSprite[i].setPosition(resManager->dx, resManager->dy); //ourPlayer
			}
			else
			{
				playerSprite[i].setPosition(enemy_dx[i], enemy_dy[i]); //enemy
				healthbarBackground[i].setPosition(playerSprite[i].getPosition() - sf::Vector2f(50, 70));
				healthbarBackground[i].setSize(sf::Vector2f(200/2, 10));
				healthbarBackground[i].setFillColor(sf::Color::Red);
				healthbar[i].setPosition(playerSprite[i].getPosition() - sf::Vector2f(50, 70));
				healthbar[i].setSize(sf::Vector2f(health_value[i]/2, 10));
				healthbar[i].setFillColor(sf::Color::Green);
			}
			if (health_value[i] == 200) playerSprite[i].setTexture(angry);
			if(health_value[i] < 200) playerSprite[i].setTexture(playerTexture);
			if(health_value[i] == 0) playerSprite[i].setTexture(dead);
			if(resManager->health == 200) playerSprite[i].setTexture(angry);
			if(resManager->health < 200) playerSprite[i].setTexture(playerTexture);
			if(resManager->health == 0) playerSprite[i].setTexture(dead);
			resManager->window.draw(playerSprite[i]);
			resManager->window.draw(healthbarBackground[i]);
			resManager->window.draw(healthbar[i]);
		}

	}
	resManager->window.draw(health);
	resManager->window.display();
}

void RenderSystem::tearDown()
{

}

bool isOnSprite(sf::Sprite *heroSprite, sf::Sprite *itemSprite)
{
	sf::Vector2f heroSpritePos = heroSprite->getPosition();
	sf::Vector2f itemSpritePos = sf::Vector2f(itemSprite->getPosition().x, itemSprite->getPosition().y);
	if (itemSprite->getGlobalBounds().contains(heroSpritePos))
	{
		LOG_INFO(1, "IS ON SPRITE!");
		return true;
	}
	else
	{
		return false;
	}
}

void RenderSystem::lookAtMouse(sf::Sprite *sprite)
{
	sf::Vector2f spritePos = sprite->getPosition();
	sf::Vector2f mousePos = resManager->window.mapPixelToCoords(sf::Mouse::getPosition(resManager->window)) + sf::Vector2f(50, 50);

	// now we have both the sprite position and the cursor
	// position lets do the calculation so our sprite will
	// face the position of the mouse
	const float PI = 3.14159265;

	float dx = spritePos.x - mousePos.x;
	float dy = spritePos.y - mousePos.y;

	float rotation = (atan2(dy, dx)) * 180 / PI;
	sprite->setRotation(rotation);
}

void RenderSystem::updateHero()
{
	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		//std::cout << "PLAYER ID["<< i <<"] ="<< playerIds[i] << std::endl;
	}

	cameraView.setCenter(playerSprite[resManager->our_player_id].getPosition());
	healthbarBackground[resManager->our_player_id].setPosition(playerSprite[resManager->our_player_id].getPosition() - sf::Vector2f(GAME_WIDTH / 2.1, GAME_HEIGHT / 2.1) - sf::Vector2f(2,2));
	healthbar[resManager->our_player_id].setPosition(playerSprite[resManager->our_player_id].getPosition()-sf::Vector2f(GAME_WIDTH/2.1 , GAME_HEIGHT/2.1));
	health.setString(std::to_string(resManager->health)+" HP");
	health.setPosition(healthbar[resManager->our_player_id].getPosition()+sf::Vector2f(2,2));
	resManager->window.setView(cameraView);
	//playerSprite.move(resManager->dx, resManager->dy);
	//isOnSprite(&playerSprite[resManager->player_ID], &swordSprite);
	
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		if (isOnSprite)
		{
			swordPicked = 1;
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//action here
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{	//1 space 2 left 3 right

		sf::Vector2f mousePos = resManager->window.mapPixelToCoords(sf::Mouse::getPosition(resManager->window));
		//sf::Vector2i mousePos = sf::Mouse::getPosition(resManager->window);
		ResourceManager::messageSendbox.emplace(MessageData(1, "actionInd"));
		ResourceManager::messageSendbox.back().addParameter("inputId", 3);
		ResourceManager::messageSendbox.back().addParameter("absMouseCoordX", mousePos.x);
	    ResourceManager::messageSendbox.back().addParameter("absMouseCoordY", mousePos.y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		sf::Vector2f mousePos = resManager->window.mapPixelToCoords(sf::Mouse::getPosition(resManager->window));
		ResourceManager::messageSendbox.emplace(MessageData(1, "actionInd"));
		ResourceManager::messageSendbox.back().addParameter("inputId", 1);
		ResourceManager::messageSendbox.back().addParameter("absMouseCoordX", mousePos.x);
		ResourceManager::messageSendbox.back().addParameter("absMouseCoordY", mousePos.y);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		//action here
	}
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) resManager->dy = -10;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::W)) resManager->dy = 10;
	else resManager->dy = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) resManager->dx = -10;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A)) resManager->dx = 10;
	else resManager->dx = 0;
	*/
	
}

void InputSystem::tearDown()
{

}

DataSystem::DataSystem(ResourceManager* rm)
{
	resManager = rm;
};

void DataSystem::init()
{

}

void DataSystem::update()
{
	for (int i = 0; i < ResourceManager::messageInbox.size(); ++i) {
		switch (ResourceManager::messageInbox.front().getMsgId())
		{
			case 0:
			{
				//loginAck
				resManager->our_player_id = ResourceManager::messageInbox.front().getIntParameter("objectId");
				playerIds[resManager->our_player_id] = resManager->our_player_id;
			}
				break;
			case 1:
			{

			}
				break;
			case 2:
			{
				playerGotUpdate[ResourceManager::messageInbox.front().getIntParameter("objectId")] = true;
				playerSpeed[ResourceManager::messageInbox.front().getIntParameter("objectId")] = ResourceManager::messageInbox.front().getDoubleParameter("objectSpeed");
				if (resManager->our_player_id == ResourceManager::messageInbox.front().getIntParameter("objectId"))
				{	//data for our player
					resManager->health = ResourceManager::messageInbox.front().getIntParameter("health");
					resManager->dx = ResourceManager::messageInbox.front().getDoubleParameter("absPositionCoordX");
					resManager->dy = ResourceManager::messageInbox.front().getDoubleParameter("absPositionCoordY");
					resManager->target_x = ResourceManager::messageInbox.front().getDoubleParameter("absTargetCoordX");
					resManager->target_y = ResourceManager::messageInbox.front().getDoubleParameter("absTargetCoordY");
				}
				else
				{
					int ID = ResourceManager::messageInbox.front().getIntParameter("objectId");
					//std::cout << "ENEMY_ID = " << ResourceManager::messageInbox.front().getIntParameter("objectId") << std::endl;
					playerIds[ID] = ID;
					health_value[ResourceManager::messageInbox.front().getIntParameter("objectId")] = ResourceManager::messageInbox.front().getIntParameter("health");
					enemy_dx[ResourceManager::messageInbox.front().getIntParameter("objectId")] = ResourceManager::messageInbox.front().getDoubleParameter("absPositionCoordX");
					enemy_dy[ResourceManager::messageInbox.front().getIntParameter("objectId")] = ResourceManager::messageInbox.front().getDoubleParameter("absPositionCoordY");
					enemy_target_dx[ResourceManager::messageInbox.front().getIntParameter("objectId")] = ResourceManager::messageInbox.front().getDoubleParameter("absTargetCoordX");
					enemy_target_dy[ResourceManager::messageInbox.front().getIntParameter("objectId")] = ResourceManager::messageInbox.front().getDoubleParameter("absTargetCoordY");
				}
			}
				break;
			case 3:
			{//Introd
				int ID = ResourceManager::messageInbox.front().getIntParameter("objectId");
				playerIds[ID] = ID;
			}
			break;
			case 4:
			{//logout
				playerIds[ResourceManager::messageInbox.front().getIntParameter("objectId")] = -1;
			}
				break;
			default:
				break;
		}
		ResourceManager::messageInbox.pop();
	}
}

void DataSystem::tearDown()
{
}