#include "DoublePointBuff.h"
#include "Player.h"

sf::Texture DoublePointBuff::texture;
sf::SoundBuffer DoublePointBuff::sfxBuffer;
sf::Sound DoublePointBuff::sfx;

DoublePointBuff::DoublePointBuff(float x, float y)
: Buff(texture, x, y) {}

bool DoublePointBuff::loadTexture() {
    return texture.loadFromFile("assets/image/buff/2xmultiplier.png");
}

bool DoublePointBuff::loadSfx() {
    bool ok = sfxBuffer.loadFromFile("assets/audio/doublepointsfx.mp3");
    if (ok) {
        sfx.setBuffer(sfxBuffer);
        sfx.setVolume(80.f);
    }
    return ok;
}

void DoublePointBuff::apply(Player& player) {
    player.activateDoublePoint();
    player.updateTexture();
    sfx.stop();
    sfx.play();
}