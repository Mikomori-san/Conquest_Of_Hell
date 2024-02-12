//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../DebugDraw.h"
#include "../../Enums/Boss_Animationtype.h"
#include "../../Enums/Enemy_Animationtype.h"
#include "../../Enums/MeeleIndicator_Animationtype.h"
#include "../../Enums/Player_Animationtype.h"
#include "../Collision_Components/RectCollisionCP.h"
#include "../StatsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "GraphicsCP.h"

template <typename Animationtype>
class AnimatedGraphicsCP : public GraphicsCP {
public:
    AnimatedGraphicsCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture, std::vector<int> animationTypeFramesCountVar, float animationSpeed, Animationtype aniType)
        : GraphicsCP(gameObject, id, texture), animationTypeFramesCount(animationTypeFramesCountVar), animationSpeed(animationSpeed),
            TILING_X([this]() {
                int max = 0;
                for (auto& it : animationTypeFramesCount)
                {
                    if (it > max)
                        max = it;
                }
                return max;
            }()), 
            TILING_Y(animationTypeFramesCount.size()),
            m_animationType(aniType)
    {}

    ~AnimatedGraphicsCP() = default;

    std::string getComponentId() override { return componentId; }
    void setComponentId(std::string id) override { this->componentId = id; }
    void init() override;
    void update(float deltaTime) override;
    void setSprite(std::shared_ptr<sf::Texture> texture) override;
    void setAnimationType(Animationtype type);
    void setAnimationSpeed(float speed) { if(toggleAllowance) this->animationSpeed = speed; }
    float getAnimationSpeed() { return this->animationSpeed; }
    int getAnimationFrame() { return this->animationFrame; }
    Animationtype getAnimationType() { return m_animationType; }
    sf::Sprite& getSprite() override { return *sprite; }
    void toggleAnimationLock() { if(toggleAllowance) animationLock = animationLock ? false : true; }
    void resetAnimationTimeIndex() { animationTimeIndex = 0; }
    void resetAnimationFrame() { animationFrame = 0; }
    bool isAnimationLock() { return this->animationLock; }
    void setHit() { isHit = true; }
    void setDying() { isDying = true; std::cout << "Dying" << std::endl; }
    void setColor(sf::Color col) { sprite->setColor(col); colorTimer = 0; }

private:
    std::vector<int> animationTypeFramesCount;
    Animationtype m_animationType;
    float animationTimeIndex = 0;
    float animationSpeed;
    int animationFrame;
    const int TILING_X;
    const int TILING_Y;
    void doAnimation();
    void doHitStuff();
    void doDeathStuff();
    bool animationLock = false;
    bool textureRectOriginalSet = false;
    bool isDying;
    bool isHit;
    bool setLastAnimation;
    bool animationFrameZero;
    sf::IntRect originalIntRect;
    Animationtype lastAnimationType;
    float oldAnimationSpeed;
    bool toggleAllowance;
    float colorTimer;
    const float COLOR_THRESHOLD = 0.5f;
};

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::init()
{
    colorTimer = 0;
    if (!textureRectOriginalSet)
    {
        originalIntRect = sprite->getTextureRect();
        textureRectOriginalSet = true;
    }

    sprite->setTextureRect(sf::IntRect(
        0,
        0,
        originalIntRect.width / TILING_X,
        originalIntRect.height / TILING_Y
    ));

    isHit = false;
    isDying = false;
    setLastAnimation = false;
    animationFrameZero = false;
    toggleAllowance = true;
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::update(float deltaTime)
{
    colorTimer += deltaTime;

    if (colorTimer >= COLOR_THRESHOLD)
    {
        sprite->setColor(sf::Color::White);
    }

    animationTimeIndex += deltaTime * animationSpeed;
    if (isDying)
    {
        doDeathStuff();
    }
    else
    {
        doHitStuff();
    }
    
    doAnimation();

    if (!gameObject.expired())
    {
        std::shared_ptr<TransformationCP> transform;
        std::shared_ptr<GameObject> go = gameObject.lock();

        transform = std::dynamic_pointer_cast<TransformationCP>(go->getComponentsOfType<TransformationCP>().at(0));

        if (transform)
        {
            sprite->setPosition(sf::Vector2f(transform->getPosition().x + sprite->getLocalBounds().width / 2, transform->getPosition().y + sprite->getLocalBounds().height / 2));
            sprite->setRotation(transform->getRotation());
            sprite->setScale(transform->getScale(), transform->getScale());
            sprite->setOrigin(sf::Vector2f(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2));
        }
    }
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::setSprite(std::shared_ptr<sf::Texture> texture)
{
    this->sprite->setTexture(*texture);
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::setAnimationType(Animationtype type)
{
    if (!animationLock && !isDying && !isHit)
        this->m_animationType = type;
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::doAnimation()
{
    animationFrame = (int)animationTimeIndex % animationTypeFramesCount[m_animationType];

    if (animationFrame != 0)
        animationFrameZero = true;

    sf::IntRect textureRect;
    textureRect.left = animationFrame * sprite->getTextureRect().width;
    textureRect.top = static_cast<int>(m_animationType) * sprite->getTextureRect().height;
    textureRect.width = sprite->getTextureRect().width;
    textureRect.height = sprite->getTextureRect().height;

    sprite->setTextureRect(textureRect);
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::doHitStuff()
{
    if (isHit && (std::is_same_v<Animationtype, Enemy_Animationtype> || std::is_same_v<Animationtype, Boss_Animationtype>))
    {
        if (!setLastAnimation)
        {
            toggleAllowance = false;

            resetAnimationTimeIndex();
            setLastAnimation = true;
            animationFrameZero = false;
            lastAnimationType = m_animationType;
            animationLock = true;
            std::cout << "Reset AnimationFrame to 1" << std::endl;
            animationFrame = 0;
            oldAnimationSpeed = animationSpeed;
            animationSpeed *= 3;

            if (!gameObject.expired())
            {
                auto go = gameObject.lock();
                auto trans = go->getComponentsOfType<TransformationCP>().at(0);
                trans->setVelocity(0);
                trans->toggleVelLock();
                std::cout << "Toggling VelLock to true!" << std::endl;
            }
        }

        if (std::is_same_v<Animationtype, Enemy_Animationtype>)
        {
            if (m_animationType == Enemy_Animationtype::AttackLeft || m_animationType == Enemy_Animationtype::HitLeft || m_animationType == Enemy_Animationtype::IdleLeft || m_animationType == Enemy_Animationtype::ReactLeft || m_animationType == Enemy_Animationtype::WalkLeft)
            {
                m_animationType = (Animationtype)Enemy_Animationtype::HitLeft;
            }
            else
            {
                m_animationType = (Animationtype)Enemy_Animationtype::HitRight;
            }
        }
        else if (std::is_same_v<Animationtype, Boss_Animationtype>)
        {
            m_animationType = (Animationtype)Boss_Animationtype::Scream;
        }

        if (animationFrame == animationTypeFramesCount[m_animationType] - 1)
        {
            m_animationType = lastAnimationType;
            isHit = false;
            animationLock = false;
            setLastAnimation = false;
            animationSpeed = oldAnimationSpeed;

            toggleAllowance = true;

            if (!gameObject.expired())
            {
                auto go = gameObject.lock();
                auto trans = go->getComponentsOfType<TransformationCP>().at(0);
                trans->setVelocity(trans->getOriginalVelocity());
                trans->toggleVelLock();
                std::cout << "Toggling Vel Lock to false!" << std::endl;
            }
        }
    }
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::doDeathStuff()
{
    if (isDying)
    {
        if (!setLastAnimation)
        {
            toggleAllowance = false;

            resetAnimationTimeIndex();
            setLastAnimation = true;
            animationFrameZero = false;
            lastAnimationType = m_animationType;
            animationLock = true;
            animationFrame = 0;
            animationSpeed *= 2;

            if (!gameObject.expired())
            {
                auto go = gameObject.lock();
                auto trans = go->getComponentsOfType<TransformationCP>().at(0);
                trans->setVelocity(0);
                trans->toggleVelLock();
                std::cout << "Toggling VelLock to true!" << std::endl;
            }
        }

        if (std::is_same_v<Animationtype, Enemy_Animationtype>)
        {
            if (m_animationType == Enemy_Animationtype::AttackLeft || m_animationType == Enemy_Animationtype::HitLeft || m_animationType == Enemy_Animationtype::IdleLeft || m_animationType == Enemy_Animationtype::ReactLeft || m_animationType == Enemy_Animationtype::WalkLeft)
            {
                m_animationType = (Animationtype)Enemy_Animationtype::DieLeft;
            }
            else
            {
                m_animationType = (Animationtype)Enemy_Animationtype::DieRight;
            }
        }
        else if (std::is_same_v<Animationtype, Player_Animationtype>)
        {
            if (m_animationType == Player_Animationtype::LeftAttack || m_animationType == Player_Animationtype::Left || m_animationType == Player_Animationtype::LeftDodge || m_animationType == Player_Animationtype::LeftIdle)
            {
                m_animationType = (Animationtype)Player_Animationtype::LeftDeath;
            }
            else
            {
                m_animationType = (Animationtype)Player_Animationtype::RightDeath;
            }
        }
        else
        {
            m_animationType = (Animationtype)Boss_Animationtype::Death;
        }

        if (animationFrame == animationTypeFramesCount[m_animationType] - 1)
        {
            gameObject.lock()->getComponentsOfType<StatsCP>().at(0)->setDeath();
        }
    }
}