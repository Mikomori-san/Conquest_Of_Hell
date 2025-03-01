//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Components/AI_Pathfinding/AStarCP.h"
#include "../Components/AI_Pathfinding/AISpriteUpdateCP.h"
#include "../Components/AI_Pathfinding/ControllerCP.h"
#include "../Components/AI_Pathfinding/SteeringCP.h"
#include "../Components/Boss/BossAttackCP.h"
#include "../Components/Boss/ScreenShakeCP.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Collision_Components/RigidBodyCP.h"
#include "../Components/DecisionHandlerCP.h"
#include "../Components/Enemy_Components/EnemyAttackCP.h"
#include "../Components/Graphics_Components/AnimatedGraphicsCP.h"
#include "../Components/Graphics_Components/HealthbarCP.h"
#include "../Components/Input_Components/MovementInputGamepadCP.h"
#include "../Components/Input_Components/MovementInputWASDCP.h"
#include "../Components/Player_Components/DashCP.h"
#include "../Components//Player_Components/PlayerAttackCP.h"
#include "../Components/Render_Components/LayerCP.h"
#include "../Components/Render_Components/SpriteRenderCP.h"
#include "../Components/Spawner_Components/SpawnerCP.h"
#include "../Components/StatsCP.h"
#include "../Components/Transformation_Components/TransformationCP.h"
#include "../Components/UI/ControlsUI.h"
#include "../Components/UI/StatusEffectCP.h"
#include "../DebugDraw.h"
#include "../Enums/Boss_Animationtype.h"
#include "../Enums/Enemy_Animationtype.h"
#include "../Enums/GamepadButton.h"
#include "../Enums/MeeleIndicator_Animationtype.h"
#include "../GameObject.h"
#include "../Manager/AssetManager.h"
#include "../Manager/PhysicsManager.h"
#include "../Manager/RenderManager.h"
#include "../Manager/GameStateManager.h"
#include "GameplayState.h"
#include <iostream>



void GameplayState::init(sf::RenderWindow& rWindow)
{
	m_close = false;
	
	m_originalView = rWindow.getView();

	this->m_window.reset(&rWindow, [](sf::RenderWindow*) {});

	DebugDraw::getInstance().initialize(*m_window);

	m_spriteSheetCounts["Player1"] = { 15, 15, 8, 8, 11, 11, 15, 15, 15, 15 };
	m_spriteSheetCounts["Skeleton"] = {11, 11, 13, 13, 18, 18, 4, 4, 8, 8, 15, 15, 15, 15};
	m_spriteSheetCounts["Boss"] = { 4,6,5,9,6,7,2,7 };

	AssetManager::getInstance().loadMusic("Clash_Of_Titans", "Assets\\Music\\Clash_of_Titans.mp3");
	m_cot = AssetManager::getInstance().Music["Clash_Of_Titans"];
	m_cot->setVolume(2);
	m_cot->setLoop(true);
	m_cot->play();

	AssetManager::getInstance().loadMusic("Sword_Hit", "Assets\\Sounds\\sword_hit.wav");
	AssetManager::getInstance().loadMusic("Player_Hit", "Assets\\Sounds\\player_hit.wav");
	AssetManager::getInstance().loadMusic("Dodge", "Assets\\Sounds\\swoosh.wav");
	AssetManager::getInstance().loadMusic("Charm", "Assets\\Sounds\\charmed.wav");
	AssetManager::getInstance().loadMusic("Bones", "Assets\\Sounds\\bone_plump.wav");
	AssetManager::getInstance().loadMusic("Punch", "Assets\\Sounds\\punch.wav");

	loadMap("game.tmj", sf::Vector2f());
	
	sf::Vector2f playerPos;
	
	for (auto& go : m_gameObjects)
	{
		go->init();

		if (go->getId().find("Player") != std::string::npos)
		{
			playerPos = go->getComponentsOfType<TransformationCP>().at(0)->getPosition();
		}

		for (auto& renderCPs : go->getComponentsOfType<RenderCP>()) 
		{
			RenderManager::getInstance().addToLayers(renderCPs);
		}
	}
	
	m_slainBoss = false;
	m_slainPlayer = false;
	
	m_view = sf::View(playerPos, sf::Vector2f(m_window->getSize().x / 1.5f, m_window->getSize().y / 1.5f));// FOR PLAYER VIEW SETTING
	m_window->setView(m_view);
}
void GameplayState::exit()
{
	m_cot->stop();
	for (auto& go : m_gameObjects)
	{
		go.reset();
	}
	m_gameObjects.clear();

	for (auto& comp : RenderManager::getInstance().getLayers())
	{
		comp.reset();
	}

	RenderManager::getInstance().getLayers().clear();
	DebugDraw::getInstance().unload();
}

void GameplayState::update(float deltaTime)
{
	auto removeCondition = [this](const std::shared_ptr<GameObject>& go) {
		if (go->getId().find("Player") != std::string::npos)
		{
			if (go->getComponentsOfType<StatsCP>().at(0)->hasDied())
			{
				m_slainPlayer = true;
			}
			m_view = sf::View(go->getComponentsOfType<TransformationCP>().at(0)->getPosition(), sf::Vector2f(m_window->getSize().x / 1.5f, m_window->getSize().y / 1.5f));
		}
		else if (go->getId().find("Boss") != std::string::npos)
		{
			if (go->getComponentsOfType<StatsCP>().at(0)->hasDied())
			{
				m_slainBoss = true;
			}
		}
		else
		{
			auto stats = go->getComponentsOfType<StatsCP>();
			if (stats.size() > 0)
			{
				return stats.at(0)->hasDied();
			}
		}
		return false;
	};

	m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(), removeCondition), m_gameObjects.end());

	if (m_slainBoss)
	{
		hasWon = true;
		m_view = m_originalView;
		m_window->setView(m_view);
		GameStateManager::getInstance().setState("Win", *m_window);
	}
	if (m_slainPlayer)
	{
		hasLost = true;
		m_view = m_originalView;
		m_window->setView(m_view);
		GameStateManager::getInstance().setState("Loose", *m_window);
	}

	m_window->setView(m_view);												// FOR PLAYER VIEW SETTING

	std::vector<std::shared_ptr<RenderCP>> renderCPs;

	std::vector<sf::Vector2i> skelPos;
	int i = 0;
	for (auto& go : m_gameObjects)
	{
		if (go->getId().find("Skeleton") != std::string::npos)
		{
			sf::Vector2f pos = go->getComponentsOfType<TransformationCP>().at(0)->getPosition();
			skelPos.push_back(sf::Vector2i(pos.x / m_mapTileSize, pos.y / m_mapTileSize));
			i++;

			if (i == 4)
			{
				break;
			}
		}

		if (go->getId().find("Boss") != std::string::npos)
		{
			go->getComponentsOfType<ScreenShakeCP>().at(0)->updateScreen(m_view);
		}
	}

	for (auto& go : m_gameObjects)
	{
		if (auto astar = go->getComponentsOfType<AStarCP>(); astar.size() > 0)
		{
			astar.at(0)->updatePositions(skelPos);
		}

		go->update(deltaTime);
		
		for (auto& renderCP : go->getComponentsOfType<RenderCP>())
		{
			renderCPs.push_back(renderCP);
		}
	}

	RenderManager::getInstance().resetLayers(renderCPs);

	PhysicsManager::getInstance().update(m_gameObjects, deltaTime);
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

	for(auto& layer : map->getLayers())
	{
		sf::err() << "Load layer: " << layer.getName() << " with width: "
			<< layer.getSize().x << " and height: " << layer.getSize().y << std::endl;

		const int size = layer.getSize().x * layer.getSize().y;

		int layerNr = layer.getProp("LayerNr")->getValue<int>();

		layerCP = std::make_shared<LayerCP>(mapGO, "Layer" + std::to_string(layerNr), m_window, layerNr, std::vector<std::shared_ptr<sf::Sprite>>());
		
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

			if (layerNr == 2 && gid != 0)
			{
				unMovablePositions.push_back(sf::Vector2i(i / layer.getSize().x, i % layer.getSize().x));
			}

			// get tile set for tile and allocate the corresponding tileSet texture
			const auto* const tileSet = map->getTilesetByGid(gid);
			const sf::Vector2i tileSize(map->getTileSize().x, map->getTileSize().y);
			sf::Texture& texture = *m_tileSetTexture[tileSet->getName()];
			
			m_mapTileSize = tileSize.x;

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

	this->m_gameObjects.push_back(mapGO);

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
				createSpawner(object, group, aStarGridSize, unMovablePositions, m_mapTileSize);
			}
			else if (object.getProp("ObjectGroup")->getValue<std::string>() == "Boss")
			{
				createBoss(object, group);
			}
		}
	}

	for (auto& go : m_gameObjects)
	{
		if (go->getId().find("Boss") != std::string::npos)
		{
			for (auto& go1 : m_gameObjects)
			{
				if (go1->getId().find("Player") != std::string::npos)
				{
					std::shared_ptr<Component> attack = std::make_shared<BossAttackCP>(BossAttackCP(go, "BossAttackCP", go1));
					go->addComponent(attack);
					
					if (go1->getComponentsOfType<MovementInputGamepadCP>().size() > 0)
					{
						go1->getComponentsOfType<PlayerAttackCP<GamepadButton>>().at(0)->setBoss(go);
					}
					else
					{
						go1->getComponentsOfType < PlayerAttackCP < sf::Keyboard::Key>> ().at(0)->setBoss(go);
					}
				}
			}
			break;
		}
	}
	//sort gameObjects
	int playerIndex = 0;
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->getId().find("Player") != std::string::npos)
		{
			playerIndex = i;
			break;
		}
	}
	for (int i = playerIndex; i > 0; i--)
	{
		std::shared_ptr<GameObject> temp = m_gameObjects[i];
		m_gameObjects[i] = m_gameObjects[i - 1];
		m_gameObjects[i - 1] = temp;
	}

}

void GameplayState::createSpawner(tson::Object& object, tson::Layer group, sf::Vector2i& aStarGridSize, std::vector<sf::Vector2i>& unMovablePositions, int mapTileSize)
{
	
	GameObjectPtr enemy1 = createEnemies(object, group, object.getProp("EnemyNr")->getValue<std::string>());
	GameObjectPtr enemy2 = createEnemies(object, group, object.getProp("EnemyNr")->getValue<std::string>() + object.getProp("EnemyNr")->getValue<std::string>());

	std::string id = "Spawner " + object.getProp("SpawnerNr")->getValue<std::string>();
	std::shared_ptr<GameObject> spawnerTemp = std::make_shared<GameObject>(id);
	          
	std::string enemyName = object.getProp("EnemyName")->getValue<std::string>();
	int maxEnemy = object.getProp("MaxEnemies")->getValue<int>();
	float spawnTime = object.getProp("SpawnTime")->getValue<float>();

	std::vector<std::shared_ptr<GameObject>>& gameObjectsRef = m_gameObjects;

	//AssetManager::getInstance().loadTexture("start")

	std::shared_ptr<SpawnerCP> spawnerCP = std::make_shared<SpawnerCP>(gameObjectsRef, enemy1, enemy2, spawnerTemp, "SpawnerCP", enemyName, maxEnemy, spawnTime, aStarGridSize, unMovablePositions, mapTileSize);

	spawnerTemp->addComponent(spawnerCP);

	m_gameObjects.push_back(spawnerTemp);

}
void GameplayState::createBoundary(tson::Object& object, tson::Layer group)
{
	std::string id = "Boundary " + object.getProp("Name")->getValue<std::string>();

	std::shared_ptr<GameObject> boundaryTemp = std::make_shared<GameObject>(id);

	std::shared_ptr<RectCollisionCP> boundaryCollisionCP = std::make_shared<RectCollisionCP>(boundaryTemp, "BoundaryCollisionCP",
		sf::Vector2f(object.getSize().x, object.getSize().y),
		object.getProp("isTrigger")->getValue<bool>(), 1
	);
	boundaryTemp->addComponent(boundaryCollisionCP);
	
	sf::Vector2f pos(sf::Vector2f(object.getPosition().x, object.getPosition().y));
	std::shared_ptr<TransformationCP> transCP = std::make_shared<TransformationCP>(boundaryTemp, "EnemyTransformationCP", pos, object.getRotation(), object.getSize().x);
	transCP->setVelocity(0);
	boundaryTemp->addComponent(transCP);
	//std::weak_ptr<GameObject> go, std::string id, const float newMass, const float newInvMass, sf::Vector2f vel
	std::shared_ptr<RigidBodyCP> rigid = std::make_shared<RigidBodyCP>(boundaryTemp, id, 1,1,sf::Vector2f(1,1));
	boundaryTemp->addComponent(rigid);
	m_gameObjects.push_back(boundaryTemp);
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
		enemyTemp, "EnemySpriteCP", *AssetManager::getInstance().Textures.at(texName), m_spriteSheetCounts[object.getProp("EnemyName")->getValue<std::string>()], ANIMATION_SPEED, aniType
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
		object.getProp("isTrigger")->getValue<bool>(), 0.8f
	);
	enemyTemp->addComponent(enemyCollisionCP);

	float mass = object.getProp("Mass")->getValue<float>();
	std::shared_ptr<RigidBodyCP> enemyRigidBodyCP = std::make_shared<RigidBodyCP>(enemyTemp, "EnemyRigidBodyCP", mass, mass == 0.f ? 0.f : 1.f / mass,
		transCP->getDirection() * transCP->getVelocity()
	);
	enemyTemp->addComponent(enemyRigidBodyCP);

	std::shared_ptr<SpriteRenderCP> enemyRenderCP = std::make_shared<SpriteRenderCP>(enemyTemp, "EnemyRenderCP", m_window, group.getProp("LayerNr")->getValue<int>());
	enemyTemp->addComponent(enemyRenderCP);

	int hp = object.getProp("Health")->getValue<int>();
	int dmg = 25; //object.getProp("Damage")->getValue<int>();
	std::shared_ptr<StatsCP> enemyStats = std::make_shared<StatsCP>(enemyTemp, "EnemyStatsCP", hp, dmg, "Enemy");
	enemyTemp->addComponent(enemyStats);

	if (!AssetManager::getInstance().Textures["healthbar"])
	{
		AssetManager::getInstance().loadTexture("healthbar", "Assets\\Textures\\health_bar.png");
	}

	std::shared_ptr<HealthbarCP> health = std::make_shared<HealthbarCP>(enemyTemp, "HealthbarCP", *AssetManager::getInstance().Textures.at("healthbar"));
	enemyTemp->addComponent(health);

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
		playerTemp, "PlayerSpriteCP", *AssetManager::getInstance().Textures.at("PlayerTexture"), m_spriteSheetCounts[playerTemp->getId()], PLAYER_ANIMATION_SPEED, Player_Animationtype::RightIdle
	);

	playerTemp->addComponent(playerGraphicsCP);

	std::shared_ptr<RectCollisionCP> playerCollisionCP = std::make_shared<RectCollisionCP>(playerTemp, "PlayerCollisionCP",
		sf::Vector2f(object.getSize().x, object.getSize().y),
		object.getProp("isTrigger")->getValue<bool>(), 1
	);
	playerTemp->addComponent(playerCollisionCP);
	
	float mass = object.getProp("Mass")->getValue<float>();
	std::shared_ptr<RigidBodyCP> playerRigidBodyCP = std::make_shared<RigidBodyCP>(playerTemp, "PlayerRigidBodyCP", mass, mass == 0.f ? 0.f : 1.f / mass,
		transCP->getDirection() * transCP->getVelocity()
	);
	playerTemp->addComponent(playerRigidBodyCP);

	std::shared_ptr<SpriteRenderCP> playerRenderCP = std::make_shared<SpriteRenderCP>(playerTemp, "PlayerRenderCP", m_window, group.getProp("LayerNr")->getValue<int>());
	playerTemp->addComponent(playerRenderCP);
	
	std::vector<std::weak_ptr<GameObject>> weak;
	int attackRange = object.getProp("AttackRange")->getValue<int>();

	if(!GameStateManager::getInstance().gamepadUse()){
		std::shared_ptr<MovementInputWASDCP> movementInputCP = std::make_shared<MovementInputWASDCP>(
			playerTemp, "MovementInputCP"
		);
		playerTemp->addComponent(movementInputCP);

		std::shared_ptr<DashCP<sf::Keyboard::Key>> dashCP = std::make_shared<DashCP<sf::Keyboard::Key>>(playerTemp, "SpaceDashCP", sf::Keyboard::Space, PLAYER_ANIMATION_SPEED);
		playerTemp->addComponent(dashCP);

		std::shared_ptr<PlayerAttackCP<sf::Keyboard::Key>> playerAttackCP = std::make_shared<PlayerAttackCP<sf::Keyboard::Key>>(playerTemp, "PlayerAttackCP", attackRange, weak, sf::Keyboard::LAlt);
		playerTemp->addComponent(playerAttackCP);
		if (!AssetManager::getInstance().Textures["keyboardControls"])
		{
			AssetManager::getInstance().loadTexture("keyboardControls", "Assets\\UI\\Keyboard.png");
		}
		std::shared_ptr<ControlsUI> ui = std::make_shared<ControlsUI>(playerTemp, "ControlsUI", *AssetManager::getInstance().Textures.at("keyboardControls"), m_window);
		playerTemp->addComponent(ui);
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
		if (!AssetManager::getInstance().Textures["gamepadControls"])
		{
			AssetManager::getInstance().loadTexture("gamepadControls", "Assets\\UI\\Gamepad.png");
		}
		std::shared_ptr<ControlsUI> ui = std::make_shared<ControlsUI>(playerTemp, "ControlsUI", *AssetManager::getInstance().Textures.at("gamepadControls"), m_window);
		playerTemp->addComponent(ui);
	}

	std::shared_ptr<DecisionHandlerCP> decHandler = std::make_shared<DecisionHandlerCP>(playerTemp, "PlayerDecisionHandlerCP");
	playerTemp->addComponent(decHandler);

	int hp = object.getProp("Health")->getValue<int>();
	int damage = object.getProp("Damage")->getValue<int>();
	std::shared_ptr<StatsCP> playerStats = std::make_shared<StatsCP>(playerTemp, "PlayerStatsCP", hp, damage, "Player");
	playerTemp->addComponent(playerStats);

	if (!AssetManager::getInstance().Textures["healthbar"])
	{
		AssetManager::getInstance().loadTexture("healthbar", "Assets\\Textures\\health_bar.png");
	}

	std::shared_ptr<HealthbarCP> health = std::make_shared<HealthbarCP>(playerTemp, "HealthbarCP", *AssetManager::getInstance().Textures.at("healthbar"));
	playerTemp->addComponent(health);

	if (!AssetManager::getInstance().Textures["stun"])
	{
		AssetManager::getInstance().loadTexture("stun", "Assets\\UI\\stun.png");
	}
	std::shared_ptr<StatusEffectCP> statusCP = std::make_shared<StatusEffectCP>(playerTemp, "StatusEffectCP", *AssetManager::getInstance().Textures.at("stun"));
	playerTemp->addComponent(statusCP);

	m_gameObjects.push_back(playerTemp);
}

void GameplayState::createBoss(tson::Object& object, tson::Layer group)
{
	int idNr = object.getProp("BossNr")->getValue<int>();
	std::string stringId = object.getProp("BossName")->getValue<std::string>();
	stringId += '0' + idNr;

	std::shared_ptr<GameObject> bossTemp = std::make_shared<GameObject>(stringId);

	const int ANIMATION_SPEED = object.getProp("AnimationSpeed")->getValue<int>();

	std::string texName = "";

	if (bossTemp->getId().find("Boss") != std::string::npos)
	{
		texName = "BossTexture";
	}

	if (!AssetManager::getInstance().Textures[texName])
	{
		AssetManager::getInstance().loadTexture(texName, object.getProp("BossTexture")->getValue<std::string>());
	}

	std::shared_ptr<AnimatedGraphicsCP<Boss_Animationtype>> bossGraphicsCP = std::make_shared<AnimatedGraphicsCP<Boss_Animationtype>>(
		bossTemp, "BossSpriteCP", *AssetManager::getInstance().Textures.at(texName), m_spriteSheetCounts[object.getProp("BossName")->getValue<std::string>()], 4, Boss_Animationtype::Idle
	);
	bossTemp->addComponent(bossGraphicsCP);
	
	//std::shared_ptr<ScreenShakeCP> screenShakeCP = std::make_shared<ScreenShakeCP>(bossGraphicsCP, bossTemp, "ScreenShakeCP", window, 10.f, 1.5f, 500.f);

	std::shared_ptr<ScreenShakeCP> screenShakeCP = std::make_shared<ScreenShakeCP>(bossGraphicsCP, bossTemp, "ScreenShakeCP", m_window);
	bossTemp->addComponent(screenShakeCP);

	const float VELOCITY = object.getProp("Velocity")->getValue<int>();
	sf::Vector2f pos(sf::Vector2f(object.getPosition().x, object.getPosition().y));

	std::shared_ptr<TransformationCP> transCP = std::make_shared<TransformationCP>(bossTemp, "BossTransformationCP", pos, object.getRotation(), object.getSize().x);
	transCP->setOriginalVelocity(VELOCITY);
	transCP->setBackupVel();
	transCP->setScale(2.f);
	
	bossTemp->addComponent(transCP);

	std::shared_ptr<RectCollisionCP> bossCollisionCP = std::make_shared<RectCollisionCP>(bossTemp, "BossCollisionCP",
		sf::Vector2f(bossGraphicsCP->getSprite().getTextureRect().getSize().x,
			bossGraphicsCP->getSprite().getTextureRect().getSize().y
		),
		object.getProp("isTrigger")->getValue<bool>(), 0.5
	);
	bossTemp->addComponent(bossCollisionCP);

	float mass = object.getProp("Mass")->getValue<float>();
	std::shared_ptr<RigidBodyCP> bossRigidBodyCP = std::make_shared<RigidBodyCP>(bossTemp, "BossRigidBodyCP", mass, mass == 0.f ? 0.f : 1.f / mass,
		transCP->getDirection() * transCP->getVelocity()
	);
	bossTemp->addComponent(bossRigidBodyCP);

	std::shared_ptr<SpriteRenderCP> bossRenderCP = std::make_shared<SpriteRenderCP>(bossTemp, "BossRenderCP", m_window, group.getProp("LayerNr")->getValue<int>());
	bossTemp->addComponent(bossRenderCP);

	int hp = object.getProp("Health")->getValue<int>();
	std::shared_ptr<StatsCP> bossStats = std::make_shared<StatsCP>(bossTemp, "BossStatsCP", hp, 25, "Boss");
	bossTemp->addComponent(bossStats);

	if (!AssetManager::getInstance().Textures["healthbar"])
	{
		AssetManager::getInstance().loadTexture("healthbar", "Assets\\Textures\\health_bar.png");
	}

	std::shared_ptr<HealthbarCP> health = std::make_shared<HealthbarCP>(bossTemp, "HealthbarCP", *AssetManager::getInstance().Textures.at("healthbar"));
	bossTemp->addComponent(health);
	m_gameObjects.push_back(bossTemp);
}