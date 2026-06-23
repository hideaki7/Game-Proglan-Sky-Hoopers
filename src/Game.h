#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <stack>
#include "constants.h"
#include "state/State.h" 

class Game {
public:
    Game();
    void run();
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);
    sf::RenderWindow& getWindow();
    State* getCurrentState();
    bool isStateStackEmpty() const { return states.empty(); }

    // BGM control — lives in Game so it persists across state changes
    sf::Music& getBgm() { return bgm; }
    void startBgm();
    void pauseBgm();
    void resumeBgm();
    
private:
    sf::RenderWindow window;
    std::stack<std::unique_ptr<State>> states;
    sf::Music bgm;
    
    void handleEvents();
    void update(float dt);
    void render();
    
};

#endif