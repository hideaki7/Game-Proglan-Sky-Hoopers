#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include "constants.h"

class Player;

class Platform {
public:
    enum PlatformType {
        NORMAL,
        CRACKED,
        SPIKE
    };

    Platform(float x, float y, int type);
    int getId() const;
    void update(float dx, float dy);
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& playerBounds, GravityMode gravity);
    void onCollision(Player& player);

    static bool loadTextures();

    float getY() const;
    float getX() const;
    PlatformType getType() const;
    bool isBroken() const;

private:
    sf::Sprite sprite;
    PlatformType type;
    int touchCount;
    bool broken;
    int id;

    static int nextId;

    static sf::Texture normalTexture;
    static sf::Texture crackedTexture;
    static sf::Texture spikeTexture;
};

#endif
