#ifndef DOUBLEPOINTBUFF_H
#define DOUBLEPOINTBUFF_H

#include "Buff.h"
#include <SFML/Audio.hpp>

class DoublePointBuff : public Buff {
public:
    DoublePointBuff(float x, float y);
    void apply(Player& player) override;
    std::string getType() const override { return "DoublePoint"; }

    static sf::Texture texture;
    static bool loadTexture();

    static sf::SoundBuffer sfxBuffer;
    static sf::Sound sfx;
    static bool loadSfx();
};

#endif