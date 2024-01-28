#include "stdafx.h"
#include "GameplayState.h"
#include "../GameObject.h"
#include "../Components/Render_Components/LayerCP.h"
#include "../Manager/RenderManager.h"
#include "../Manager/AssetManager.h"
#include "../Components/Graphics_Components/AnimatedGraphicsCP.h"
#include "../Components/Transformation_Components/TransformationCP.h"
#include "../Components/Input_Components/MovementInputArrowsCP.h"
#include "../Components/Input_Components/MovementInputWASDCP.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Render_Components/SpriteRenderCP.h"
#include "../DebugDraw.h"
#include "../Components/Collision_Components/RigidBodyCP.h"
#include "../Manager/PhysicsManager.h"
#include "../Components/StatsCP.h"
#include "../Components/DecisionHandlerCP.h"
#include "../Components/AI_Pathfinding/ControllerCP.h"
#include "../Components/AI_Pathfinding/SteeringCP.h"
#include "../Components/AI_Pathfinding/AISpriteUpdateCP.h"
#include "../Components/AI_Pathfinding/AStarCP.h"

void GameplayState::init(sf::RenderWindow& rWindow)
{
	this->window.reset(&rWindow, [](sf::RenderWindow*) {});

	this->window->setSize(sf::Vector2u(975, 650));

	DebugDraw::getInstance().initialize(*window);

	spriteSheetCounts["Player1"] = { 6, 6, 6, 6, 6, 6, 6, 6 };
	spriteSheetCounts["Player2"] = { 6, 6, 6, 6, 6, 6, 6, 6 };
	spriteSheetCounts["Impostor"] = { 6, 6, 6, 6, 6, 6, 6, 6 };
	spriteSheetCounts["Crawler"] = { 1, 1, 1, 1, 4, 4, 4, 4 };

	loadMap("game.tmj", sf::Vector2f());
	
	currentLayer = 0;
	std::cout << "Current Layer: " << currentLayer << std::endl;

	for (auto& go : gameObjects)
	{
		go->init();
		
		for (auto& renderCPs : go->getComponentsOfType<RenderCP>()) 
		{
			RenderManager::getInstance().addToLayers(renderCPs);
		}
	}
	
	window->setView(sf::View(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), (sf::Vector2f)window->getSize()));
}

void GameplayState::exit()
{
	for (auto& go : gameObjects)
	{
		go.reset();
	}
	gameObjects.clear();

	for (auto& comp : RenderManager::getInstance().getLayers())
	{
		comp.reset();
	}

	RenderManager::getInstance().getLayers().clear();
	maxLayer = 0;
	DebugDraw::getInstance().unload();
}

void GameplayState::update(float deltaTime)
{
	for (auto& go : gameObjects)
	{
		go->update(deltaTime);
		/*
		if (go->getId().find("Player1") != std::string::npos)
		{
			std::cout << "Player Pos: " << go->getComponentsOfType<TransformationCP>().at(0)->getPosition().x << ", " << go->getComponentsOfType<TransformationCP>().at(0)->getPosition().y << std::endl;
		}
		*/
	}

	//checkAreaBorders();

	checkPlayerLayer();

	PhysicsManager::getInstance().update(gameObjects, deltaTime);
}

void GameplayState::render()
{
	RenderManager::getInstance().render();
}

void GameplayState::loadMap(std::string name, const sf::Vector2f& offset)
{
	tson::Tileson t;
	const std::unique_ptr<tson::Map> map = t.parse(m_resourcePath / name);
	std::shared_ptr<GameObject> mapGO = std::make_shared<GameObject>("Map");

	if (map->getStatus() == tson::ParseStatus::OK)
	{

		sf::err() << "Load tile map with size: " << map->getSize().x << ", "
			<< map->getSize().y << " and tile size: " << map->getTileSize().x
			<< ", " << map->getTileSize().y << std::endl;

		for (auto& tileSet : map->getTilesets())
		{
			fs::path tileSetPath = tileSet.getImage().u8string();

			const auto texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile((m_resourcePath / tileSetPath).string()))
			{
				sf::err() << "Could not load texture " << m_resourcePath / tileSetPath << std::endl;
			}
			m_tileSetTexture[tileSet.getName()] = texture;
		}
	}
	else
	{
		std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
	}

	// go through all layers

	std::shared_ptr<LayerCP> layerCP;
	sf::Vector2i aStarGridSize;
	std::vector<sf::Vector2i> unMovablePositions;
	int mapTileSize;

	for(auto& layer : map->getLayers())
	{
		sf::err() << "Load layer: " << layer.getName() << " with width: "
			<< layer.getSize().x << " and height: " << layer.getSize().y << std::endl;

		const int size = layer.getSize().x * layer.getSize().y;

		int layerNr = layer.getProp("LayerNr")->getValue<int>();

		layerCP = std::make_shared<LayerCP>(mapGO, "Layer" + std::to_string(layerNr), window, layerNr, std::vector<std::shared_ptr<sf::Sprite>>());
		
		if (layerNr == 0) // 0 => Background, get all Grid Stats (width, height) for A*
		{
			aStarGridSize.x = layer.getSize().y;
			aStarGridSize.y = layer.getSize().x;
		}

		// iterate over all elements/tiles in the layer
		for (int i = 0; i < size; i++)
		{
			const auto gid = layer.getData()[i];

			if (gid == 0)
			{
				// 0 means there is no tile at this position.
				continue;
			}

			if (layerNr == 3 && gid != 0)
			{
				unMovablePositions.push_back(sf::Vector2i(i / layer.getSize().x, i % layer.getSize().x));
			}

			// get tile set for tile and allocate the corresponding tileSet texture
			const auto* const tileSet = map->getTilesetByGid(gid);
			const sf::Vector2i tileSize(map->getTileSize().x, map->getTileSize().y);
			sf::Texture& texture = *m_tileSetTexture[tileSet->getName()];
			
			mapTileSize = tileSize.x;

			// calculate position of tile
			sf::Vector2f position;
			position.x = i % layer.getSize().x * static_cast<float>(tileSize.x);
			position.y = i / layer.getSize().x * static_cast<float>(tileSize.y);
			position += offset;

			// number of tiles in tile set texture (horizontally)
			const int tileCountX = texture.getSize().x / tileSize.x;

			// calculate 2d idx of tile in tile set texture
			const int idx = gid - tileSet->getFirstgid();
			const int idxX = idx % tileCountX;
			const int idxY = idx / tileCountX;

			// calculate source area of tile in tile set texture
			sf::IntRect source(idxX * tileSize.x, idxY * tileSize.y, tileSize.x, tileSize.y);	

			// create tile (Sprite) and put it into the appropriate layer.
			auto sprite = std::make_shared<sf::Sprite>();
			sprite->setTexture(texture);
			sprite->setTextureRect(source);
			sprite->setPosition(position.x, position.y);

			layerCP->getLayer().push_back(sprite);

		}
		mapGO->addComponent(layerCP);
		maxLayer++;
	}

	this->gameObjects.push_back(mapGO);

	for (auto group : map->getLayers())
	{
		// go over all objects per layer
		for (auto object : group.getObjects())
		{
			sf::Vector2f position(object.getPosition().x, object.getPosition().y);
			position += offset;

			// example to get a texture rectangle for a sprite
			sf::FloatRect bounds(position.x, position.y, object.getSize().x, object.getSize().y);
			// TODO: check out game.tmj and there the content contained within <object group name="Object Layer 1">
			// there you can see the properties that you can parse, that should help you set up the sprites
			std::cout << object.getRotation() << std::endl;
			std::cout << object.getType() << std::endl;
			
			if (object.getProp("ObjectGroup")->getValue<std::string>() == "Player")
			{
				createPlayers(object, group);
			}
			else if (object.getProp("ObjectGroup")->getValue<std::string>() == "PatrolPoint")
			{
				createPatrolPoints(object, group);
			}
			else if (object.getProp("ObjectGroup")->getValue<std::string>() == "Enemy")
			{
				createEnemies(object, group);
			}
			else if (object.getProp("ObjectGroup")->getValue<std::string>() == "Boundary")
			{
				createBoundary(object, group);
			}
		}
	}

	for (auto& go : gameObjects)
	{
		if (auto stats = go->getComponentsOfType<StatsCP>(); stats.size() > 0)
		{
			std::string objectType = stats.at(0)->getObjectType();
			if (objectType == "Enemy")
			{
				std::vector<std::shared_ptr<GameObject>> players;
				std::vector<std::shared_ptr<GameObject>> patrolPoints;
				for (auto& go1 : gameObjects)
				{
					if (go1->getId().find("Player") != std::string::npos)
					{
						players.push_back(go1);
					}
					else if (go1->getId().find("PatrolPoint") != std::string::npos)
					{
						if (go1->getId().find(stats.at(0)->ifEnemyGetPatrolPoints()) != std::string::npos)
						{
							patrolPoints.push_back(go1);
						}
					}
				}

				std::shared_ptr<ControllerCP> enemyAIController = std::make_shared<ControllerCP>(go, "EnemyControllerCP", players, patrolPoints);
				go->addComponent(enemyAIController);

				std::shared_ptr<AStarCP> enemyAStarCP = std::make_shared<AStarCP>(go, "EnemyAStarCP", std::vector<std::vector<int>>(aStarGridSize.x, std::vector<int>(aStarGridSize.y, 0)), unMovablePositions, sf::Vector2f(0, 0), mapTileSize);
				go->addComponent(enemyAStarCP);

				std::shared_ptr<SteeringCP> enemySteeringCP = std::make_shared<SteeringCP>(go, "EnemySteeringCP");
				go->addComponent(enemySteeringCP);

				std::shared_ptr<AISpriteUpdateCP> enemyAISpriteUpdateCP = std::make_shared<AISpriteUpdateCP>(go, "EnemyAISpriteUpdateCP");
				go->addComponent(enemyAISpriteUpdateCP);
			}
		}
	}
}

void GameplayState::checkAreaBorders()
{
	auto left = window->getView().getCenter().x - window->getView().getSize().x / 2;
	auto top = window->getView().getCenter().y - window->getView().getSize().y / 2;
	auto right = window->getView().getCenter().x + window->getView().getSize().x / 2;
	auto bottom = window->getView().getCenter().y + window->getView().getSize().y / 2;

	std::shared_ptr<TransformationCP> transf;
	std::shared_ptr<RectCollisionCP> collision;

	for (auto& go : gameObjects)
	{
		//Check for both players
		if (go->getId().find("Player") != std::string::npos)
		{
			if ((transf = go->getComponentsOfType<TransformationCP>().at(0)) && (collision = go->getComponentsOfType<RectCollisionCP>().at(0)))
			{
				if (transf && collision)
				{
					if (transf->getPosition().y > bottom - collision->getCollisionRect().height / 2)
						transf->setPosition(sf::Vector2f(transf->getPosition().x, bottom - collision->getCollisionRect().height / 2));

					if (transf->getPosition().y < top + collision->getCollisionRect().height / 2)
						transf->setPosition(sf::Vector2f(transf->getPosition().x, top + collision->getCollisionRect().height / 2));

					if (transf->getPosition().x > right - collision->getCollisionRect().width / 2)
						transf->setPosition(sf::Vector2f(right - collision->getCollisionRect().width / 2, transf->getPosition().y));

					if (transf->getPosition().x < left + collision->getCollisionRect().width / 2)
						transf->setPosition(sf::Vector2f(left + collision->getCollisionRect().width / 2, transf->getPosition().y));
				}
			}
		}
	}
}

void GameplayState::checkPlayerLayer()
{
	if (InputManager::getInstance().getKeyDown(sf::Keyboard::Home) || InputManager::getInstance().getKeyDown(sf::Keyboard::End))
	{
		if (InputManager::getInstance().getKeyDown(sf::Keyboard::Home) && currentLayer < maxLayer)
		{
			currentLayer++;
			std::cout << "Current Layer: " << currentLayer << std::endl;
		}
		else if(currentLayer > 0)
		{
			currentLayer--;
			std::cout << "Current Layer: " << currentLayer << std::endl;
		}
	}

	if (InputManager::getInstance().getKeyDown(sf::Keyboard::PageUp) || InputManager::getInstance().getKeyDown(sf::Keyboard::PageDown))
	{
		if (InputManager::getInstance().getKeyDown(sf::Keyboard::PageUp) && currentLayer < maxLayer)
		{
			for (auto& cp : RenderManager::getInstance().getLayers())
			{
				if (cp->getLayerNr() == currentLayer)
				{
					cp->setLayerNr(currentLayer + 1);
				}
				else if (cp->getLayerNr() == currentLayer + 1)
				{
					cp->setLayerNr(currentLayer);
				}
			}
			currentLayer++;
			std::cout << "New current Layer: " << currentLayer << std::endl;
		}
		else if (InputManager::getInstance().getKeyDown(sf::Keyboard::PageDown) && currentLayer > 0)
		{
			for (auto& cp : RenderManager::getInstance().getLayers())
			{
				if (cp->getLayerNr() == currentLayer)
				{
					cp->setLayerNr(currentLayer - 1);
				}
				else if (cp->getLayerNr() == currentLayer - 1)
				{
					cp->setLayerNr(currentLayer);
				}
			}
			currentLayer--;
			std::cout << "New current Layer: " << currentLayer << std::endl;
		}
	}
}

void GameplayState::createPatrolPoints(tson::Object& object, tson::Layer group)
{
	std::string id = "PatrolPoint" + object.getProp("PatrolNr")->getValue<std::string>();
	std::shared_ptr<GameObject> patrolPointTemp = std::make_shared<GameObject>(id);

	sf::Vector2f pos(sf::Vector2f(object.getPosition().x, object.getPosition().y));
	std::shared_ptr<TransformationCP> transCP = std::make_shared<TransformationCP>(patrolPointTemp, "PatrolTransformationCP", pos, object.getRotation(), object.getSize().x);
	patrolPointTemp->addComponent(transCP);

	gameObjects.push_back(patrolPointTemp);
}

void GameplayState::createBoundary(tson::Object& object, tson::Layer group)
{
	std::string id = "Boundary " + object.getProp("Name")->getValue<std::string>();

	std::shared_ptr<GameObject> boundaryTemp = std::make_shared<GameObject>(id);

	std::shared_ptr<RectCollisionCP> boundaryCollisionCP = std::make_shared<RectCollisionCP>(boundaryTemp, "BoundaryCollisionCP",
		sf::Vector2f(object.getSize().x, object.getSize().y),
		object.getProp("isTrigger")->getValue<bool>()
	);
	boundaryTemp->addComponent(boundaryCollisionCP);
	
	sf::Vector2f pos(sf::Vector2f(object.getPosition().x, object.getPosition().y));
	std::shared_ptr<TransformationCP> transCP = std::make_shared<TransformationCP>(boundaryTemp, "EnemyTransformationCP", pos, object.getRotation(), object.getSize().x);
	transCP->setVelocity(0);
	boundaryTemp->addComponent(transCP);

	gameObjects.push_back(boundaryTemp);
}

void GameplayState::createEnemies(tson::Object& object, tson::Layer group)
{
	int idNr = object.getProp("EnemyNr")->getValue<int>();
	std::string stringId = object.getProp("EnemyName")->getValue<std::string>();
	stringId += '0' + idNr;

	std::shared_ptr<GameObject> enemyTemp = std::make_shared<GameObject>(stringId);

	const int ANIMATION_SPEED = object.getProp("AnimationSpeed")->getValue<int>();

	std::string texName = "";

	if (enemyTemp->getId().find("Impostor") != std::string::npos)
	{
		texName = "ImpostorTexture";
	}
	else if (enemyTemp->getId().find("Crawler") != std::string::npos)
	{
		texName = "CrawlerTexture";
	}

	if (!AssetManager::getInstance().Textures[texName])
	{
		AssetManager::getInstance().loadTexture(texName, object.getProp("EnemyTexture")->getValue<std::string>());
	}
	Animationtype aniType;
	if (enemyTemp->getId().find("Impostor") != std::string::npos)
	{
		aniType = Animationtype::Right;
	}
	else if (enemyTemp->getId().find("Crawler") != std::string::npos)
	{
		aniType = Animationtype::LeftUp;
	}
	std::shared_ptr<AnimatedGraphicsCP> enemyGraphicsCP = std::make_shared<AnimatedGraphicsCP>(
		enemyTemp, "EnemySpriteCP", *AssetManager::getInstance().Textures.at(texName), spriteSheetCounts[object.getProp("EnemyName")->getValue<std::string>()], ANIMATION_SPEED, aniType
	);

	enemyTemp->addComponent(enemyGraphicsCP);

	const float VELOCITY = object.getProp("Velocity")->getValue<int>();
	sf::Vector2f pos(sf::Vector2f(object.getPosition().x, object.getPosition().y));

	std::shared_ptr<TransformationCP> transCP = std::make_shared<TransformationCP>(enemyTemp, "EnemyTransformationCP", pos, object.getRotation(), object.getSize().x);
	transCP->setOriginalVelocity(VELOCITY);
	transCP->setBackupVel();

	enemyTemp->addComponent(transCP);

	std::shared_ptr<RectCollisionCP> enemyCollisionCP = std::make_shared<RectCollisionCP>(enemyTemp, "EnemyCollisionCP",
		sf::Vector2f(enemyGraphicsCP->getSprite().getTextureRect().getSize().x,
			enemyGraphicsCP->getSprite().getTextureRect().getSize().y
		),
		object.getProp("isTrigger")->getValue<bool>()
	);
	enemyTemp->addComponent(enemyCollisionCP);

	float mass = object.getProp("Mass")->getValue<float>();
	std::shared_ptr<RigidBodyCP> enemyRigidBodyCP = std::make_shared<RigidBodyCP>(enemyTemp, "EnemyRigidBodyCP", mass, mass == 0.f ? 0.f : 1.f / mass,
		transCP->getDirection() * transCP->getVelocity()
	);
	enemyTemp->addComponent(enemyRigidBodyCP);

	std::shared_ptr<SpriteRenderCP> enemyRenderCP = std::make_shared<SpriteRenderCP>(enemyTemp, "EnemyRenderCP", window, group.getProp("LayerNr")->getValue<int>());
	enemyTemp->addComponent(enemyRenderCP);

	int hp = object.getProp("Health")->getValue<int>();
	std::shared_ptr<StatsCP> enemyStats = std::make_shared<StatsCP>(enemyTemp, "EnemyStatsCP", hp, "Enemy");
	enemyStats->ifEnemyAddPatrolPoints(object.getProp("PatrolNr")->getValue<std::string>());
	enemyTemp->addComponent(enemyStats);

	gameObjects.push_back(enemyTemp);
}

void GameplayState::createPlayers(tson::Object& object, tson::Layer group)
{
	int idNr = object.getProp("PlayerNr")->getValue<int>();
	std::string stringId = "Player";
	stringId += '0' + idNr;

	std::shared_ptr<GameObject> playerTemp = std::make_shared<GameObject>(stringId);

	if (!AssetManager::getInstance().Textures["PlayerTexture"])
	{
		AssetManager::getInstance().loadTexture("PlayerTexture", object.getProp("PlayerTexture")->getValue<std::string>());
	}

	const float VELOCITY = object.getProp("Velocity")->getValue<int>();
	sf::Vector2f pos(sf::Vector2f(object.getPosition().x, object.getPosition().y));

	std::shared_ptr<TransformationCP> transCP = std::make_shared<TransformationCP>(playerTemp, "PlayerTransformationCP", pos, object.getRotation(), object.getSize().x);
	transCP->setOriginalVelocity(VELOCITY);
	playerTemp->addComponent(transCP);

	const int PLAYER_ANIMATION_SPEED = object.getProp("AnimationSpeed")->getValue<int>();

	std::shared_ptr<AnimatedGraphicsCP> playerGraphicsCP = std::make_shared<AnimatedGraphicsCP>(
		playerTemp, "PlayerSpriteCP", *AssetManager::getInstance().Textures.at("PlayerTexture"), spriteSheetCounts[playerTemp->getId()], PLAYER_ANIMATION_SPEED, Animationtype::Right
	);

	playerTemp->addComponent(playerGraphicsCP);

	bool useArrowKeys = object.getProp("ArrowKeys")->getValue<bool>();
	
	std::shared_ptr<RectCollisionCP> playerCollisionCP = std::make_shared<RectCollisionCP>(playerTemp, "PlayerCollisionCP",
		sf::Vector2f(object.getSize().x, object.getSize().y),
		object.getProp("isTrigger")->getValue<bool>()
	);
	playerTemp->addComponent(playerCollisionCP);
	
	float mass = object.getProp("Mass")->getValue<float>();
	std::shared_ptr<RigidBodyCP> playerRigidBodyCP = std::make_shared<RigidBodyCP>(playerTemp, "PlayerRigidBodyCP", mass, mass == 0.f ? 0.f : 1.f / mass,
		transCP->getDirection() * transCP->getVelocity()
	);
	playerTemp->addComponent(playerRigidBodyCP);

	std::shared_ptr<SpriteRenderCP> playerRenderCP = std::make_shared<SpriteRenderCP>(playerTemp, "PlayerRenderCP", window, group.getProp("LayerNr")->getValue<int>());
	playerTemp->addComponent(playerRenderCP);

	if (useArrowKeys) {
		std::shared_ptr<MovementInputArrowsCP> movementInputCP = std::make_shared<MovementInputArrowsCP>(
			playerTemp, "MovementInputCP"
		);
		playerTemp->addComponent(movementInputCP);
	}
	else {
		std::shared_ptr<MovementInputWASDCP> movementInputCP = std::make_shared<MovementInputWASDCP>(
			playerTemp, "MovementInputCP"
		);
		playerTemp->addComponent(movementInputCP);
	}

	std::shared_ptr<DecisionHandlerCP> decHandler = std::make_shared<DecisionHandlerCP>(playerTemp, "PlayerDecisionHandlerCP");
	playerTemp->addComponent(decHandler);

	int hp = object.getProp("Health")->getValue<int>();
	std::shared_ptr<StatsCP> playerStats = std::make_shared<StatsCP>(playerTemp, "PlayerStatsCP", hp, "Player");
	playerTemp->addComponent(playerStats);

	gameObjects.push_back(playerTemp);
}