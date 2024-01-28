#pragma once
#include "GraphicsCP.h"
#include "../../Enums/Animationtype.h"

class AnimatedGraphicsCP : public GraphicsCP {
public:
    AnimatedGraphicsCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture, std::vector<int> animationTypeFramesCountVar, float animationSpeed, Animationtype aniType)
        : GraphicsCP(gameObject, id, texture), animationTypeFramesCount(animationTypeFramesCountVar), ANIMATION_SPEED(animationSpeed),
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
    sf::Sprite& getSprite() override { return *sprite; }
    
private:
    std::vector<int> animationTypeFramesCount;
    Animationtype m_animationType;
    float animationTimeIndex = 0;
    const float ANIMATION_SPEED;
    const int TILING_X;
    const int TILING_Y;
    void doAnimation();
};