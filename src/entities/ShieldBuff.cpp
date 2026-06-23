#include "ShieldBuff.h"
#include "Player.h"

sf::Texture ShieldBuff::texture;
sf::SoundBuffer ShieldBuff::sfxBuffer;
sf::Sound ShieldBuff::sfx;

ShieldBuff::ShieldBuff(float x, float y)
: Buff(texture, x, y) {}

bool ShieldBuff::loadTexture() {
    return texture.loadFromFile("assets/image/buff/shield.png");
}

bool ShieldBuff::loadSfx() {
    bool ok = sfxBuffer.loadFromFile("assets/audio/shieldsfx.mp3");
    if (ok) {
        sfx.setBuffer(sfxBuffer);
        sfx.setVolume(55.f);
    }
    return ok;
}

void ShieldBuff::apply(Player& player) {
    player.activateShield();
    player.updateTexture();
    sfx.stop();
    sfx.play();
}