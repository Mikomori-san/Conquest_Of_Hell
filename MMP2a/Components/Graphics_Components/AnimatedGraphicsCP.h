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
        : GraphicsCP(gameObject, id, texture), animationTypeFramesCount(animationTypeFramesCountVar), m_animationSpeed(animationSpeed),
            m_TILING_X([this]() {
                int max = 0;
                for (auto& it : animationTypeFramesCount)
                {
                    if (it > max)
                        max = it;
                }
                return max;
            }()), 
            m_TILING_Y(animationTypeFramesCount.size()),
            m_animationType(aniType)
    {}

    ~AnimatedGraphicsCP() = default;

    std::string getComponentId() override { return componentId; }
    void setComponentId(std::string id) override { this->componentId = id; }
    void init() override;
    void update(float deltaTime) override;
    void setSprite(std::shared_ptr<sf::Texture> texture) override;
    void setAnimationType(Animationtype type);
    void setAnimationSpeed(float speed) { if(m_toggleAllowance) this->m_animationSpeed = speed; }
    float getAnimationSpeed() { return this->m_animationSpeed; }
    int getAnimationFrame() { return this->m_animationFrame; }
    Animationtype getAnimationType() { return m_animationType; }
    sf::Sprite& getSprite() override { return *m_sprite; }
    void toggleAnimationLock() { if(m_toggleAllowance) m_animationLock = m_animationLock ? false : true; }
    void resetAnimationTimeIndex() { m_animationTimeIndex = 0; }
    void resetAnimationFrame() { m_animationFrame = 0; }
    bool isAnimationLock() { return this->m_animationLock; }
    void setHit() { m_isHit = true; }
    void setDying() { m_isDying = true; }
    void setColor(sf::Color col) { m_sprite->setColor(col); m_colorTimer = 0; }

private:
    std::vector<int> animationTypeFramesCount;
    Animationtype m_animationType;
    float m_animationTimeIndex = 0;
    float m_animationSpeed;
    int m_animationFrame;
    const int m_TILING_X;
    const int m_TILING_Y;
    void doAnimation();
    void doHitStuff();
    void doDeathStuff();
    bool m_animationLock = false;
    bool m_textureRectOriginalSet = false;
    bool m_isDying;
    bool m_isHit;
    bool m_setLastAnimation;
    bool m_animationFrameZero;
    sf::IntRect m_originalIntRect;
    Animationtype m_lastAnimationType;
    float m_oldAnimationSpeed;
    bool m_toggleAllowance;
    float m_colorTimer;
    const float m_COLOR_THRESHOLD = 0.5f;
};

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::init()
{
    m_colorTimer = 0;
    if (!m_textureRectOriginalSet)
    {
        m_originalIntRect = m_sprite->getTextureRect();
        m_textureRectOriginalSet = true;
    }

    m_sprite->setTextureRect(sf::IntRect(
        0,
        0,
        m_originalIntRect.width / m_TILING_X,
        m_originalIntRect.height / m_TILING_Y
    ));

    m_isHit = false;
    m_isDying = false;
    m_setLastAnimation = false;
    m_animationFrameZero = false;
    m_toggleAllowance = true;
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::update(float deltaTime)
{
    m_colorTimer += deltaTime;

    if (m_colorTimer >= m_COLOR_THRESHOLD)
    {
        m_sprite->setColor(sf::Color::White);
    }

    m_animationTimeIndex += deltaTime * m_animationSpeed;
    if (m_isDying)
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
            m_sprite->setPosition(sf::Vector2f(transform->getPosition().x + m_sprite->getLocalBounds().width / 2, transform->getPosition().y + m_sprite->getLocalBounds().height / 2));
            m_sprite->setRotation(transform->getRotation());
            m_sprite->setScale(transform->getScale(), transform->getScale());
            m_sprite->setOrigin(sf::Vector2f(m_sprite->getLocalBounds().width / 2, m_sprite->getLocalBounds().height / 2));
        }
    }
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::setSprite(std::shared_ptr<sf::Texture> texture)
{
    this->m_sprite->setTexture(*texture);
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::setAnimationType(Animationtype type)
{
    if (!m_animationLock && !m_isDying && !m_isHit)
        this->m_animationType = type;
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::doAnimation()
{
    m_animationFrame = (int)m_animationTimeIndex % animationTypeFramesCount[m_animationType];

    if (m_animationFrame != 0)
        m_animationFrameZero = true;

    sf::IntRect textureRect;
    textureRect.left = m_animationFrame * m_sprite->getTextureRect().width;
    textureRect.top = static_cast<int>(m_animationType) * m_sprite->getTextureRect().height;
    textureRect.width = m_sprite->getTextureRect().width;
    textureRect.height = m_sprite->getTextureRect().height;

    m_sprite->setTextureRect(textureRect);
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::doHitStuff()
{
    if (m_isHit && (std::is_same_v<Animationtype, Enemy_Animationtype> || std::is_same_v<Animationtype, Boss_Animationtype>))
    {
        if (!m_setLastAnimation)
        {
            m_toggleAllowance = false;

            resetAnimationTimeIndex();
            m_setLastAnimation = true;
            m_animationFrameZero = false;
            m_lastAnimationType = m_animationType;
            m_animationLock = true;
            m_animationFrame = 0;
            m_oldAnimationSpeed = m_animationSpeed;
            m_animationSpeed *= 3;

            if (!gameObject.expired())
            {
                auto go = gameObject.lock();
                auto trans = go->getComponentsOfType<TransformationCP>().at(0);
                trans->setVelocity(0);
                trans->toggleVelLock();
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

        if (m_animationFrame == animationTypeFramesCount[m_animationType] - 1)
        {
            m_animationType = m_lastAnimationType;
            m_isHit = false;
            m_animationLock = false;
            m_setLastAnimation = false;
            m_animationSpeed = m_oldAnimationSpeed;

            m_toggleAllowance = true;

            if (!gameObject.expired())
            {
                auto go = gameObject.lock();
                auto trans = go->getComponentsOfType<TransformationCP>().at(0);
                trans->setVelocity(trans->getOriginalVelocity());
                trans->toggleVelLock();
            }
        }
    }
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::doDeathStuff()
{
    if (m_isDying)
    {
        if (!m_setLastAnimation)
        {
            m_toggleAllowance = false;

            resetAnimationTimeIndex();
            m_setLastAnimation = true;
            m_animationFrameZero = false;
            m_lastAnimationType = m_animationType;
            m_animationLock = true;
            m_animationFrame = 0;
            m_animationSpeed *= 2;

            if (!gameObject.expired())
            {
                auto go = gameObject.lock();
                auto trans = go->getComponentsOfType<TransformationCP>().at(0);
                trans->setVelocity(0);
                trans->toggleVelLock();
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

        if (m_animationFrame == animationTypeFramesCount[m_animationType] - 1)
        {
            gameObject.lock()->getComponentsOfType<StatsCP>().at(0)->setDeath();
        }
    }
}