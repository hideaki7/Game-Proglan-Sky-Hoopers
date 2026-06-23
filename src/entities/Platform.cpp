#include "Platform.h"
#include "Player.h"
#include "constants.h"
#include <iostream>

sf::Texture Platform::normalTexture;
sf::Texture Platform::crackedTexture;
sf::Texture Platform::spikeTexture;
int Platform::nextId = 0;

bool Platform::loadTextures() {
    bool success = true;
    success &= normalTexture.loadFromFile("assets/image/platform/platform-ijo-besar.png");
    success &= crackedTexture.loadFromFile("assets/image/platform/platform-coklat-retak.png");
    success &= spikeTexture.loadFromFile("assets/image/platform/platform-biru-duri.png");
    return success;
}

Platform::Platform(float x, float y, int t)
    : type(static_cast<PlatformType>(t)), touchCount(0), broken(false), id(nextId++)
{
    switch (type) {
        case NORMAL:
            sprite.setTexture(normalTexture);
            break;
        case CRACKED:
            sprite.setTexture(crackedTexture);
            break;
        case SPIKE:
            sprite.setTexture(spikeTexture);
            break;
    }
    // Scale platform ke ukuran yang konsisten
    sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.width > 0 && bounds.height > 0) {
        sprite.setScale(PLATFORM_WIDTH / bounds.width, PLATFORM_HEIGHT / bounds.height);
    }
    sprite.setPosition(x, y);
}

void Platform::update(float dx, float dy) {
    sprite.move(dx, dy);
}

void Platform::draw(sf::RenderWindow& window) {
    if (!(type == CRACKED && broken)) {
        window.draw(sprite);
    }
}

bool Platform::checkCollision(const sf::FloatRect& playerBounds, GravityMode gravity) {
    if (type == CRACKED && broken)
        return false;
        
    sf::FloatRect platformBounds = sprite.getGlobalBounds();
    
    // SELALU cek landing vertikal (player mendarat dari atas ke platform)
    // Player physics selalu vertikal, baik di mode Vertical maupun Horizontal
    
    // Toleransi y-axis diperbesar menjadi 40 pixel untuk mencegah "tunneling" (menembus platform) 
    // saat player jatuh dari ketinggian dengan kecepatan tinggi (terutama di mode horizontal)
    return (playerBounds.left + playerBounds.width > platformBounds.left + 5) &&
           (playerBounds.left < platformBounds.left + platformBounds.width - 5) &&
           (playerBounds.top + playerBounds.height >= platformBounds.top) &&
           (playerBounds.top + playerBounds.height <= platformBounds.top + 40) &&
           (playerBounds.top < platformBounds.top);
}

void Platform::onCollision(Player& player) {
    if (type == NORMAL) {
        // Score ditangani oleh GameState berdasarkan platform ID
        player.setGrounded(true); // Ini akan memicu lompat otomatis
    } 
    else if (type == CRACKED) {
        touchCount++;
        // Score ditangani oleh GameState berdasarkan platform ID
        player.setGrounded(true); // Ini akan memicu lompat otomatis
        if (touchCount >= 2) {
            broken = true;
        }
    } 
    else if (type == SPIKE) {
        if (!player.hasShield()) {
            player.setDead();
        }
    }
}

int Platform::getId() const {
    return id;
}

float Platform::getY() const {
    return sprite.getPosition().y;
}

float Platform::getX() const {
    return sprite.getPosition().x;
}

Platform::PlatformType Platform::getType() const {
    return type;
}

bool Platform::isBroken() const {
    return broken;
}
