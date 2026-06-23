#ifndef SHIELDBUFF_H
#define SHIELDBUFF_H

#include "Buff.h"
#include <SFML/Audio.hpp>

class ShieldBuff : public Buff {
public:
    ShieldBuff(float x, float y);
    void apply(Player& player) override;
    std::string getType() const override { return "Shield"; }

    static sf::Texture texture;
    static bool loadTexture();

    static sf::SoundBuffer sfxBuffer;
    static sf::Sound sfx;
    static bool loadSfx();
};

#endif