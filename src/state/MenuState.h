#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "State.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>

class Game;
class GameState;
class GuideState;

class MenuState : public State {
public:
    explicit MenuState(Game& game);
    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    // Background
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    // Sign / logo
    sf::Texture signTexture;
    sf::Sprite signSprite;

    // Button texture (shared by all buttons)
    sf::Texture btnTexture;

    // Font for button labels
    sf::Font arcadeFont;

    struct MenuButton {
        sf::Sprite sprite;
        sf::Text label;
        sf::FloatRect area;
        std::function<void()> action;
    };
    std::vector<MenuButton> buttons;

    int hoveredIndex = -1; // track which button is hovered

    void setupButtons();
};

#endif