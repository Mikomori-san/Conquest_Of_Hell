//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Input_Components/InputCP.h"
#include "../../Enums/Enemy_Animationtype.h"

class EnemyAttackCP : public Component
{
public:
    EnemyAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<GameObject> closestPlayer, int attackRange) : Component(gameObject, id), m_closestPlayer(closestPlayer), m_attackRange(attackRange) {};

    void update(float deltaTime) override;
    std::string getComponentId() override { return this->componentId; };
    void setComponentId(std::string id) override { this->componentId = id; };
    void init() override;
    void setClosestPlayer(std::weak_ptr<GameObject> closePlayer) { this->m_closestPlayer = closePlayer; }

private:
    std::weak_ptr<GameObject> m_closestPlayer;
    int m_attackRange;
    float m_attackCD;

    bool m_hasAttacked;
    bool m_inputLocked;
    bool m_animationLocked;

    float m_attackTimer;

    int m_originalAnimationSpeed;
    Enemy_Animationtype m_lastAnimation;

    void doAttackAnimation(std::shared_ptr<AnimatedGraphicsCP<Enemy_Animationtype>> ani, std::shared_ptr<TransformationCP> transf);
    std::weak_ptr<GameObject> m_lastPlayerAttacked;
};