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
#include "../Components/Player_Components/DashCP.h"
#include "../Components//Player_Components/PlayerAttackCP.h"
#include "../Enums/Enemy_Animationtype.h"
#include "../Components/Enemy_Components/EnemyAttackCP.h"
#include <iostream>
#include "../Components/Input_Components/MovementInputGamepadCP.h"
#include "../Enums/GamepadButton.h"
#include "../Components/Spawner_Components/SpawnerCP.h"

//template<typename T>
//void GameplayState::doLeftoverComponents(T playerAttackCP, sf::Vector2i aStarGridSize, std::vector<sf::Vector2i> unMovablePositions, int mapTileSize)
//{
//	for (auto& go : gameObjects)
//	{
//		if (auto stats = go->getComponentsOfType<StatsCP>(); stats.size() > 0)
//		{
//			std::string objectType = stats.at(0)->getObjectType();
//			if (objectType == "Enemy")
//			{
//				playerAttackCP->addEnemy(go);
//
//				std::vector<std::weak_ptr<GameObject>> players;
//				for (auto& go1 : gameObjects)
//				{
//					if (go1->getId().find("Player") != std::string::npos)
//					{
//						players.push_back(go1);
//					}
//				}
//
//				std::shared_ptr<ControllerCP> enemyAIController = std::make_shared<ControllerCP>(go, "EnemyControllerCP", players);
//				go->addComponent(enemyAIController);
//
//				std::shared_ptr<AStarCP> enemyAStarCP = std::make_shared<AStarCP>(go, "EnemyAStarCP", std::vector<std::vector<int>>(aStarGridSize.x, std::vector<int>(aStarGridSize.y, 0)), unMovablePositions, sf::Vector2f(0, 0), mapTileSize);
//				go->addComponent(enemyAStarCP);
//
//				std::shared_ptr<SteeringCP> enemySteeringCP = std::make_shared<SteeringCP>(go, "EnemySteeringCP");
//				go->addComponent(enemySteeringCP);
//
//				std::shared_ptr<AISpriteUpdateCP> enemyAISpriteUpdateCP = std::make_shared<AISpriteUpdateCP>(go, "EnemyAISpriteUpdateCP");
//				go->addComponent(enemyAISpriteUpdateCP);
//
//				std::shared_ptr<EnemyAttackCP> enemyAttackCP = std::make_shared<EnemyAttackCP>(go, "EnemyAttackCP", players.at(0), 200);
//				go->addComponent(enemyAttackCP);
//			}
//		}
//	}
//}

void GameplayState::init(sf::RenderWindow& rWindow)
{
	this->window.reset(&rWindow, [](sf::RenderWindow*) {});

	//this->window->setSize(sf::Vector2u(960, 540));

	DebugDraw::getInstance().initialize(*window);

	spriteSheetCounts["Player1"] = { 15, 15, 8, 8, 11, 11, 15, 15, 15, 15 };
	spriteSheetCounts["Skeleton"] = {11, 11, 13, 13, 18, 18, 4, 4, 8, 8, 15, 15, 15, 15};

	loadMap("game.tmj", sf::Vector2f());

	for (auto& go : gameObjects)
	{
		go->init();

		for (auto& renderCPs : go->getComponentsOfType<RenderCP>()) 
		{
			RenderManager::getInstance().addToLayers(renderCPs);
		}
	}
	
	window->setView(sf::View(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 4), sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2)));

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
	DebugDraw::getInstance().unload();
}

void GameplayState::update(float deltaTime)
{
	auto removeCondition = [](const std::shared_ptr<GameObject>& go) {
		auto stats = go->getComponentsOfType<StatsCP>();
		if (stats.size() > 0)
		{
			return stats.at(0)->getHealth() <= 0;
		}
		return false;
	};

	gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), removeCondition), gameObjects.end());		// HIER WERDEN ALLE GOS MIT STATS = 0 GELÖSCHT

	std::vector<std::shared_ptr<RenderCP>> renderCPs;
	for (auto& go : gameObjects)
	{
		go->update(deltaTime);

		for (auto& renderCP : go->getComponentsOfType<RenderCP>())
		{
			renderCPs.push_back(renderCP);
		}
	}
	RenderManager::getInstance().resetLayers(renderCPs);

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
		//std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
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
			// there you can see the properties that you can parse, that should help you set up the sprites;
			
			if (object.getProp("ObjectGroup")->getValue<std::string>() == "Player")
			{
				createPlayers(object, group);
			}
			else if (object.getProp("ObjectGroup")->getValue<std::string>() == "Boundary")
			{
				createBoundary(object, group);
			}
			else if (object.getProp("ObjectGroup")->getValue<std::string>() == "Spawner")
			{
				createSpawner(object, group, aStarGridSize, unMovablePositions, mapTileSize);
			}
		}
	}
}

void GameplayState::createSpawner(tson::Object& object, tson::Layer group, sf::Vector2i& aStarGridSize, std::vector<sf::Vector2i>& unMovablePositions, int mapTileSize)
{

	
	GameObjectPtr enemy1 = createEnemies(object, group, "0");
	GameObjectPtr enemy2 = createEnemies(object, group, "1");

	//--------------------------------------------------------------------------------------------------------------------------------

	std::string id = "Spawner " + object.getProp("SpawnerNr")->getValue<std::string>();
	std::shared_ptr<GameObject> spawnerTemp = std::make_shared<GameObject>(id);

	std::string enemyName = object.getProp("EnemyName")->getValue<std::string>();
	int maxEnemy = object.getProp("MaxEnemies")->getValue<int>();
	float spawnTime = object.getProp("SpawnTime")->getValue<float>();

	std::vector<std::shared_ptr<GameObject>>& gameObjectsRef = gameObjects;

	std::shared_ptr<SpawnerCP> spawnerCP = std::make_shared<SpawnerCP>(gameObjectsRef, enemy1, enemy2, spawnerTemp, "SpawnerCP", enemyName, maxEnemy, spawnTime, aStarGridSize, unMovablePositions, mapTileSize);
	

	spawnerTemp->addComponent(spawnerCP);


	gameObjects.push_back(spawnerTemp);

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

GameObjectPtr GameplayState::createEnemies(tson::Object& object, tson::Layer group, std::string id)
{
	std::string stringId = object.getProp("EnemyName")->getValue<std::string>();
	stringId += stringId + id;

	std::shared_ptr<GameObject> enemyTemp = std::make_shared<GameObject>(stringId);

	const int ANIMATION_SPEED = object.getProp("AnimationSpeed")->getValue<int>();

	std::string texName = "";

	texName = object.getProp("EnemyName")->getValue<std::string>().append("Texture");

	if (!AssetManager::getInstance().Textures[texName])
	{
		AssetManager::getInstance().loadTexture(texName, object.getProp("EnemyTexture")->getValue<std::string>());
	}
	Enemy_Animationtype aniType = Enemy_Animationtype::IdleRight;

	std::shared_ptr<AnimatedGraphicsCP<Enemy_Animationtype>> enemyGraphicsCP = std::make_shared<AnimatedGraphicsCP<Enemy_Animationtype>>(
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
	int dmg = object.getProp("Damage")->getValue<int>();
	std::shared_ptr<StatsCP> enemyStats = std::make_shared<StatsCP>(enemyTemp, "EnemyStatsCP", hp, dmg, "Enemy");
	enemyTemp->addComponent(enemyStats);
	std::shared_ptr<GameObject>& enemyREF = enemyTemp;
	return enemyTemp;
	/*gameObjects.push_back(enemyTemp);*/
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

	std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> playerGraphicsCP = std::make_shared<AnimatedGraphicsCP<Player_Animationtype>>(
		playerTemp, "PlayerSpriteCP", *AssetManager::getInstance().Textures.at("PlayerTexture"), spriteSheetCounts[playerTemp->getId()], PLAYER_ANIMATION_SPEED, Player_Animationtype::RightIdle
	);

	playerTemp->addComponent(playerGraphicsCP);

	bool useArrowKeys = object.getProp("ArrowKeys")->getValue<bool>();
	bool useController = object.getProp("Controller")->getValue<bool>();

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
	
	std::vector<std::weak_ptr<GameObject>> weak;
	int attackRange = object.getProp("AttackRange")->getValue<int>();

	if (useArrowKeys) {
		std::shared_ptr<MovementInputArrowsCP> movementInputCP = std::make_shared<MovementInputArrowsCP>(
			playerTemp, "MovementInputCP"
		);
		playerTemp->addComponent(movementInputCP);
		std::shared_ptr<DashCP<sf::Keyboard::Key>> dashCP = std::make_shared<DashCP<sf::Keyboard::Key>>(playerTemp, "EnterDashCP", sf::Keyboard::Enter, PLAYER_ANIMATION_SPEED);
		playerTemp->addComponent(dashCP);

		std::shared_ptr<PlayerAttackCP<sf::Keyboard::Key>> playerAttackCP = std::make_shared<PlayerAttackCP<sf::Keyboard::Key>>(playerTemp, "PlayerAttackCP", attackRange, weak, sf::Keyboard::RControl);
		playerTemp->addComponent(playerAttackCP);
	}
	else if(!useController){
		std::shared_ptr<MovementInputWASDCP> movementInputCP = std::make_shared<MovementInputWASDCP>(
			playerTemp, "MovementInputCP"
		);
		playerTemp->addComponent(movementInputCP);

		std::shared_ptr<DashCP<sf::Keyboard::Key>> dashCP = std::make_shared<DashCP<sf::Keyboard::Key>>(playerTemp, "SpaceDashCP", sf::Keyboard::Space, PLAYER_ANIMATION_SPEED);
		playerTemp->addComponent(dashCP);

		std::shared_ptr<PlayerAttackCP<sf::Keyboard::Key>> playerAttackCP = std::make_shared<PlayerAttackCP<sf::Keyboard::Key>>(playerTemp, "PlayerAttackCP", attackRange, weak, sf::Keyboard::LAlt);
		playerTemp->addComponent(playerAttackCP);
	}
	else
	{
		int controllerNr = object.getProp("ControllerNr")->getValue<int>();
		std::shared_ptr<MovementInputGamepadCP> movementInputCP = std::make_shared<MovementInputGamepadCP>(playerTemp, "MovementInputCP", controllerNr);
		playerTemp->addComponent(movementInputCP);

		std::shared_ptr<DashCP<GamepadButton>> dashCP = std::make_shared<DashCP<GamepadButton>>(playerTemp, "XDashCP", GamepadButton::A, PLAYER_ANIMATION_SPEED);
		playerTemp->addComponent(dashCP);

		std::shared_ptr<PlayerAttackCP<GamepadButton>> playerAttackCP = std::make_shared<PlayerAttackCP<GamepadButton>>(playerTemp, "PlayerAttackCP", attackRange, weak, GamepadButton::X);
		playerTemp->addComponent(playerAttackCP);
	}

	std::shared_ptr<DecisionHandlerCP> decHandler = std::make_shared<DecisionHandlerCP>(playerTemp, "PlayerDecisionHandlerCP");
	playerTemp->addComponent(decHandler);

	int hp = object.getProp("Health")->getValue<int>();
	int damage = object.getProp("Damage")->getValue<int>();
	std::shared_ptr<StatsCP> playerStats = std::make_shared<StatsCP>(playerTemp, "PlayerStatsCP", hp, damage, "Player");
	playerTemp->addComponent(playerStats);

	gameObjects.push_back(playerTemp);
}