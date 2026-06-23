#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"
#include <SFML/Graphics.hpp>
#include <set>

class Player {
public:
    Player();
    
    // Movement
    void update(float dt);
    void jump();
    void move(float dx);  // Gerakan horizontal saja
    void setPosition(float x, float y);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setAutoJump(bool enable) { autoJump = enable; }
    bool isAutoJumpEnabled() const { return autoJump; }

    // Buff system
    void activateDoublePoint();
    void activateRocket();
    void activateShield();
    bool hasDoublePoint() const { return doublePoint; }
    bool hasShield() const { return shield; }
    bool isRocketing() const { return rocket; }
    int getScore() const { return score; }
    void updateTexture();
    
    // Gravity control
    void switchGravityMode(GravityMode mode);
    
    // Score & life
    void addScore(int amount);
    void setDead();

    // Platform stepping tracking
    bool hasSteppedOn(int platformId) const;
    void markStepped(int platformId);
    void clearSteppedPlatforms();
    bool isDead() const;

    // Rendering
    void draw(sf::RenderWindow& window);
    void setGrounded(bool grounded);
    bool isGrounded() const;    

    // Portal
    bool canTeleport() const;
    void markTeleported();
    void resetTeleport();
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    sf::Texture doublePointTexture;
    sf::Texture rocketTexture;
    sf::Texture shieldTexture;
    bool dead = false;
    bool rocket = false;
    bool shield = false;
    bool doublePoint = false;
    bool hasActiveBuff = false;
    bool isGrounded_ = false;
    GravityMode currentGravity = GravityMode::Vertical;
    int score = 0;
    bool autoJump = true;
    std::set<int> steppedPlatforms;
    sf::Clock buffClock;
    bool justTeleported = false;
    sf::Clock teleportCooldown;
    float rocketScoreTimer = 0.f;
    bool isFlashing = false;
    float flashTimer = 0.f;
    sf::Texture collectedTexture;
    sf::Sprite collectedSprite;
    bool isCollectedAnim = false;
    float collectedAnimTimer = 0.f;
    int collectedFrame = 0;

};

#endif