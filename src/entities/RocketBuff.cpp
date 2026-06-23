#include "RocketBuff.h"
#include "Player.h"

sf::Texture RocketBuff::texture;
sf::SoundBuffer RocketBuff::sfxBuffer;
sf::Sound RocketBuff::sfx;

RocketBuff::RocketBuff(float x, float y)
: Buff(texture, x, y) {}

bool RocketBuff::loadTexture() {
    return texture.loadFromFile("assets/image/buff/rocket.png");
}

bool RocketBuff::loadSfx() {
    bool ok = sfxBuffer.loadFromFile("assets/audio/rocketsfx.mp3");
    if (ok) {
        sfx.setBuffer(sfxBuffer);
        sfx.setVolume(100.f);
    }
    return ok;
}

void RocketBuff::apply(Player& player) {
    player.activateRocket();
    player.updateTexture();
    sfx.stop();
    sfx.play();
}