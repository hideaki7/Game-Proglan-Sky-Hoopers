#ifndef ROCKETBUFF_H
#define ROCKETBUFF_H

#include "Buff.h"
#include <SFML/Audio.hpp>

class RocketBuff : public Buff {
public:
    RocketBuff(float x, float y);
    void apply(Player& player) override;
    std::string getType() const override { return "Rocket"; }

    static sf::Texture texture;
    static bool loadTexture();

    static sf::SoundBuffer sfxBuffer;
    static sf::Sound sfx;
    static bool loadSfx();
};

#endif