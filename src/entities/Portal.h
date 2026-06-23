#ifndef PORTAL_H
#define PORTAL_H
#include "constants.h"
#include <SFML/Graphics.hpp>


class Portal {
public:
    Portal(float x, float y, GravityMode targetMode);

    void update(float dx, float dy);
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& bounds) const;
    GravityMode getTargetMode() const;

    static bool loadTextures();

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    sf::Sprite sprite;
    GravityMode targetMode;

    // Animation
    sf::Clock animClock;
    int currentFrame = 0;
    float frameDuration = 0.15f;

    int frameCols = 1;
    int frameRows = 1;
    int totalFrames = 1;

    void updateAnimation();

    static sf::Texture greenTexture;  // Portal_xX
    static sf::Texture purpleTexture; // Portal_Y
};

#endif
