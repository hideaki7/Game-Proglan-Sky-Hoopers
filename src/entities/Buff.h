#ifndef BUFF_H
#define BUFF_H

#include <SFML/Graphics.hpp>
#include <string>

class Player; // forward declaration

class Buff {
public:
    Buff(const sf::Texture& texture, float x, float y);
    virtual ~Buff() = default;

    virtual void apply(Player& player) = 0;
    virtual std::string getType() const = 0;

    void update(float dx, float dy);
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& playerBounds);
    bool isUsed() const;

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

protected:
    sf::Sprite sprite;
    bool used = false;
};

#endif