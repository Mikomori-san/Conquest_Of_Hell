#pragma once
#include "GraphicsCP.h"
#include "../../Enums/Player_Animationtype.h"
#include "../../Enums/Enemy_Animationtype.h"
#include "../../DebugDraw.h"
#include "../Transformation_Components/TransformationCP.h"
#include "../Collision_Components/RectCollisionCP.h"

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
    void setAnimationSpeed(float speed) { this->animationSpeed = speed; }
    float getAnimationSpeed() { return this->animationSpeed; }
    Animationtype getAnimationType() { return m_animationType; }
    sf::Sprite& getSprite() override { return *sprite; }
    void toggleAnimationLock() { animationLock = animationLock ? false : true; }
    void resetAnimationTimeIndex() { animationTimeIndex = 0; }
    bool isAnimationLock() { return this->animationLock; }
private:
    std::vector<int> animationTypeFramesCount;
    Animationtype m_animationType;
    AnimationtypeBoss m_bossAnimation = Idle;
    float animationTimeIndex = 0;
    float animationSpeed;
    const int TILING_X;
    const int TILING_Y;
    void doAnimation();
    bool animationLock = false;
    bool textureRectOriginalSet = false;
    sf::IntRect originalIntRect;
};

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::init()
{
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
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::update(float deltaTime)
{
    animationTimeIndex += deltaTime * animationSpeed;
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
    if (!animationLock)
        this->m_animationType = type;
}

template <typename Animationtype>
inline void AnimatedGraphicsCP<Animationtype>::doAnimation()
{
    auto animationFrame = (int)animationTimeIndex % animationTypeFramesCount[m_animationType];

    sf::IntRect textureRect;
    textureRect.left = animationFrame * sprite->getTextureRect().width;
    textureRect.top = static_cast<int>(m_animationType) * sprite->getTextureRect().height;
    textureRect.width = sprite->getTextureRect().width;
    textureRect.height = sprite->getTextureRect().height;

    sprite->setTextureRect(textureRect);
}
