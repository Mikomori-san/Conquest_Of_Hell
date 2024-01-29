#pragma once
#include "GraphicsCP.h"
#include "../../Enums/Player_Animationtype.h"

class AnimatedGraphicsCP : public GraphicsCP {
public:
    AnimatedGraphicsCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture, std::vector<int> animationTypeFramesCountVar, float animationSpeed, Player_Animationtype aniType)
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
    void setAnimationType(Player_Animationtype type);
    void setAnimationSpeed(float speed) { this->animationSpeed = speed; }
    float getAnimationSpeed() { return this->animationSpeed; }
    Player_Animationtype getAnimationType() { return m_animationType; }
    sf::Sprite& getSprite() override { return *sprite; }
    void toggleAnimationLock() { animationLock = animationLock ? false : true; }
    void resetAnimationTimeIndex() { animationTimeIndex = 0; }
private:
    std::vector<int> animationTypeFramesCount;
    Player_Animationtype m_animationType;
    float animationTimeIndex = 0;
    float animationSpeed;
    const int TILING_X;
    const int TILING_Y;
    void doAnimation();
    bool animationLock = false;
};