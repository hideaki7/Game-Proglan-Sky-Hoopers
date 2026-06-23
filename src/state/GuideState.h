#ifndef GUIDESTATE_H
#define GUIDESTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>

class GuideState : public State {
public:
    explicit GuideState(Game& game);
    void handleEvent(sf::Event& event) override;
    void update(float dt) override;  // Tambah override
    void render(sf::RenderWindow& window) override;
    
private:
    sf::Texture guideTexture;
    sf::Sprite guideSprite;
};

#endif