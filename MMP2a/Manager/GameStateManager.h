#pragma once
#include "../GameStates/GameState.h"

class GameState;

class GameStateManager
{
public:

    static GameStateManager& getInstance()
    {
        static GameStateManager instance;
        return instance;
    }

    void reg(std::string name, std::shared_ptr<GameState> state);
    void setState(std::string name, sf::RenderWindow& window);
    void update(float deltaTime);
    void render();

private:
    GameStateManager() {}
    ~GameStateManager() {}
    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator= (const GameStateManager&) = delete;

    std::shared_ptr<GameState> currentState;
    std::map<std::string, std::shared_ptr<GameState>> states;
};