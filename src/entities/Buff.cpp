#include "Buff.h"
#include "Player.h"

Buff::Buff(const sf::Texture& texture, float x, float y) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Buff::update(float dx, float dy) {
    sprite.move(dx, dy);
}

void Buff::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Buff::getPosition() const {
    return sprite.getPosition();
}

void Buff::draw(sf::RenderWindow& window) {
    if (!used)
        window.draw(sprite);
}

bool Buff::checkCollision(const sf::FloatRect& playerBounds) {
    if (!used && sprite.getGlobalBounds().intersects(playerBounds)) {
        used = true;
        return true;
    }
    return false;
}

bool Buff::isUsed() const {
    return used;
}