#include "Player.h"
#include "constants.h"
#include <stdexcept>

Player::Player() {
    if (!texture.loadFromFile("assets/image/character/Idle (32x32).png") ||
        !doublePointTexture.loadFromFile("assets/image/characterBuff/2x/Jump (32x32).png") ||
        !rocketTexture.loadFromFile("assets/image/characterBuff/rocket/Jump (32x32).png") ||
        !shieldTexture.loadFromFile("assets/image/characterBuff/shield/Fall (32x32).png") ||
        !collectedTexture.loadFromFile("assets/image/buff/Collected.png")) {
        throw std::runtime_error("Failed to load player or buff textures");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 100.f);
    
    collectedSprite.setTexture(collectedTexture);
    collectedSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    
    score = 0; // reset score setiap kali player dibuat (game baru)
}

void Player::update(float dt) {
    // Gravity SELALU pada sumbu Y (player jatuh ke bawah, lompat ke atas)
    // Tidak peduli mode gravity — physics player selalu vertikal
    if (!isGrounded_ && !rocket) {
        velocity.y += GRAVITY * dt;
        if (velocity.y > TERMINAL_VELOCITY) {
            velocity.y = TERMINAL_VELOCITY;
        }
    }

    // Saat rocket horizontal aktif, paksa velocity.y = 0 agar terbang lurus
    if (rocket && currentGravity == GravityMode::Horizontal) {
        velocity.y = 0.f;
    }

    // Deceleration SELALU pada sumbu X
    if (!rocket || currentGravity != GravityMode::Horizontal) {
        if (std::abs(velocity.x) > 0.1f) {
            float decel = PLAYER_DECELERATION * (velocity.x > 0 ? -1.f : 1.f) * dt;
            if (std::abs(decel) >= std::abs(velocity.x)) {
                velocity.x = 0.f;
            } else {
                velocity.x += decel;
            }
        } else {
            velocity.x = 0.f;
        }
    }

    // Auto score for rocket buff
    if (rocket) {
        rocketScoreTimer += dt;
        if (rocketScoreTimer >= 1.0f) {
            rocketScoreTimer -= 1.0f;
            addScore(10);
        }
    } else {
        rocketScoreTimer = 0.f;
    }

    // Cek buff timer - hanya kalau ada buff yang aktif
    if (hasActiveBuff && buffClock.getElapsedTime().asSeconds() > BUFF_DURATION) {
        if (rocket && currentGravity == GravityMode::Horizontal) {
            velocity.x = 0.f; // Stop the horizontal momentum
            velocity.y = PLAYER_JUMP_FORCE; // Beri lompatan kecil agar bisa mendarat
        }
        rocket = false;
        shield = false;
        doublePoint = false;
        hasActiveBuff = false;
        updateTexture();
    }
    
    if (justTeleported && teleportCooldown.getElapsedTime().asSeconds() > 1.0f) {
        justTeleported = false;
    }

    // Collected animation logic
    if (isCollectedAnim) {
        collectedAnimTimer += dt;
        if (collectedAnimTimer >= 0.05f) { // 0.05 detik per frame
            collectedAnimTimer -= 0.05f;
            collectedFrame++;
            if (collectedFrame >= 6) { // Ada 6 frame (192 / 32)
                isCollectedAnim = false;
            } else {
                collectedSprite.setTextureRect(sf::IntRect(collectedFrame * 32, 0, 32, 32));
            }
        }
    }

    // Flash effect logic
    if (isFlashing) {
        flashTimer -= dt;
        if (flashTimer <= 0.f) {
            isFlashing = false;
        }
    }

    sprite.move(velocity * dt);
    
    // Boundary wrap sesuai gravity mode
    if (currentGravity == GravityMode::Vertical) {
        // Mode vertikal: wrap horizontal (kiri/kanan)
        if (sprite.getPosition().x < -sprite.getGlobalBounds().width) {
            sprite.setPosition(WINDOW_WIDTH, sprite.getPosition().y);
        }
        else if (sprite.getPosition().x > WINDOW_WIDTH) {
            sprite.setPosition(0, sprite.getPosition().y);
        }
    }
    // Mode horizontal: TIDAK ada wrap — game over di kiri ditangani GameState
}

void Player::jump() {
    if (isGrounded_) {
        // SELALU lompat ke atas (sumbu Y negatif)
        velocity.y = PLAYER_JUMP_FORCE * (rocket ? 1.5f : 1.0f);
        isGrounded_ = false;
    }
}

void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::addScore(int amount) {
    score += (doublePoint ? amount * 2 : amount);
}

bool Player::isDead() const {
    return dead;
}

void Player::setDead() {
    dead = true;
}

// Movement — SELALU kontrol horizontal (sumbu X)
void Player::move(float dx) {
    if (rocket && currentGravity == GravityMode::Horizontal) return; // ignore horizontal input when rocketing right
    velocity.x = dx * PLAYER_MOVE_SPEED;
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

// Buff system

void Player::activateDoublePoint() { 
    doublePoint = true;
    hasActiveBuff = true;
    buffClock.restart();
}

void Player::activateRocket() { 
    rocket = true;
    hasActiveBuff = true;
    buffClock.restart();
    rocketScoreTimer = 0.f;
    if (currentGravity == GravityMode::Vertical) {
        // SELALU dorong ke atas (sumbu Y negatif)
        velocity.y = -ROCKET_BOOST * 0.6f;
    } else {
        // Dorong ke kanan
        velocity.x = ROCKET_BOOST * 0.6f;
        velocity.y = 0.f;
    }
}

void Player::activateShield() { 
    shield = true;
    hasActiveBuff = true;
    buffClock.restart();
}

void Player::updateTexture() {
    isFlashing = true;
    flashTimer = 0.5f;

    // Trigger collected animation
    isCollectedAnim = true;
    collectedAnimTimer = 0.f;
    collectedFrame = 0;
    collectedSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    if (rocket) {
        sprite.setTexture(rocketTexture);
    } 
    else if (shield) {
        sprite.setTexture(shieldTexture);
    }
    else if (doublePoint) {
        sprite.setTexture(doublePointTexture);
    }
    else {
        sprite.setTexture(texture);
    }
}


// Gravity control
void Player::switchGravityMode(GravityMode mode) {
    currentGravity = mode;
    velocity = sf::Vector2f(0, 0);

    // Jika sedang menggunakan rocket, pastikan arah lesatannya diperbarui sesuai mode baru
    if (rocket) {
        if (currentGravity == GravityMode::Vertical) {
            velocity.y = -ROCKET_BOOST * 0.6f;
        } else {
            velocity.x = ROCKET_BOOST * 0.6f;
        }
    }
}

// Render
void Player::draw(sf::RenderWindow& window) {
    // Pastikan warna asli putih penuh
    sprite.setColor(sf::Color::White);
    window.draw(sprite);

    // Flash putih bersinar saat transisi buff
    if (isFlashing) {
        sf::Sprite flashSprite = sprite;
        float intensity = std::max(0.f, std::min(1.f, flashTimer / 0.5f));
        // Taruh warna putih dengan alpha sesuai intensitas timer
        flashSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * intensity)));
        // Gunakan BlendAdd agar warnanya ditambahkan ke layar (efek bercahaya terang)
        window.draw(flashSprite, sf::BlendAdd);
    }

    // Draw collected animation di atas player
    if (isCollectedAnim) {
        collectedSprite.setPosition(sprite.getPosition());
        window.draw(collectedSprite);
    }
}

void Player::setGrounded(bool grounded) {
    isGrounded_ = grounded;
    if (grounded) {
        // SELALU reset velocity Y
        velocity.y = 0;
        
        // Rocket TIDAK boleh dibatalkan oleh grounding
        // Rocket hanya selesai lewat timer (BUFF_DURATION)
        if (!rocket) {
            // Lompat otomatis ketika mendarat (hanya kalau bukan rocket)
            if (autoJump) {
                this->jump();
            }
        }
    }
}

bool Player::isGrounded() const {
    return isGrounded_;
}

bool Player::canTeleport() const {
    return !justTeleported || teleportCooldown.getElapsedTime().asSeconds() > 1.0f;
}

void Player::markTeleported() {
    justTeleported = true;
    teleportCooldown.restart();
}

void Player::resetTeleport() {
    justTeleported = false;
}

bool Player::hasSteppedOn(int platformId) const {
    return steppedPlatforms.count(platformId) > 0;
}

void Player::markStepped(int platformId) {
    steppedPlatforms.insert(platformId);
}

void Player::clearSteppedPlatforms() {
    steppedPlatforms.clear();
}