#pragma once
#include "../Component.h"
#include "../../Enums/Enemy_Animationtype.h"
#include "../Input_Components/InputCP.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"

class EnemyAttackCP : public Component
{
public:
    EnemyAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<GameObject> closestPlayer, int attackRange) : Component(gameObject, id), closestPlayer(closestPlayer), attackRange(attackRange) {};

    void update(float deltaTime) override;
    std::string getComponentId() override { return this->componentId; };
    void setComponentId(std::string id) override { this->componentId = id; };
    void init() override;
    void setClosestPlayer(std::weak_ptr<GameObject> closePlayer) { this->closestPlayer = closePlayer; }

private:
    std::weak_ptr<GameObject> closestPlayer;
    int attackRange;
    float attackCD;

    bool hasAttacked;
    bool inputLocked;
    bool animationLocked;

    float attackTimer;

    int originalAnimationSpeed;
    Enemy_Animationtype lastAnimation;

    void doAttackAnimation(std::shared_ptr<AnimatedGraphicsCP<Enemy_Animationtype>> ani, std::shared_ptr<TransformationCP> transf);
    std::weak_ptr<GameObject> lastPlayerAttacked;
};