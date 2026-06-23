#include "Portal.h"

sf::Texture Portal::greenTexture;
sf::Texture Portal::purpleTexture;

bool Portal::loadTextures() {
    bool ok1 = greenTexture.loadFromFile("assets/image/portal/Portal_xX.png");
    bool ok2 = purpleTexture.loadFromFile("assets/image/portal/Portal_Y.png");
    return ok1 && ok2;
}

Portal::Portal(float x, float y, GravityMode targetMode)
    : targetMode(targetMode)
{
    if (targetMode == GravityMode::Horizontal) {
        sprite.setTexture(greenTexture);
        frameCols = 3;
        frameRows = 2;
        totalFrames = 6;
    } else {
        sprite.setTexture(purpleTexture);
        frameCols = 2;
        frameRows = 3;
        totalFrames = 6;
    }

    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setPosition(x, y);
}

void Portal::update(float dx, float dy) {
    updateAnimation();
    sprite.move(dx, dy);
}

void Portal::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Portal::getPosition() const {
    return sprite.getPosition();
}

void Portal::updateAnimation() {
    if (animClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;
        int col = currentFrame % frameCols;
        int row = currentFrame / frameCols;
        sprite.setTextureRect(sf::IntRect(col * 32, row * 32, 32, 32));
        animClock.restart();
    }
}

void Portal::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Portal::checkCollision(const sf::FloatRect& bounds) const {
    return sprite.getGlobalBounds().intersects(bounds);
}

GravityMode Portal::getTargetMode() const {
    return targetMode;
}

