#include "Game.h"
#include "state/MenuState.h"
#include <SFML/Audio.hpp>
#include <vector>

Game::Game()
: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sky Hoopers", sf::Style::Close | sf::Style::Titlebar)
{
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view);

    {
        sf::SoundBuffer warmupBuf;
        std::vector<sf::Int16> silence(4410, 0);
        warmupBuf.loadFromSamples(silence.data(), silence.size(), 1, 44100);
        sf::Sound warmup(warmupBuf);
        warmup.setVolume(0.f);
        warmup.play();
        sf::sleep(sf::milliseconds(50));
        warmup.stop();
    }
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void Game::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void Game::popState() {
    if (!states.empty()) {
        states.pop();
    }
}

void Game::changeState(std::unique_ptr<State> state) {
    if (!states.empty()) {
        states.pop();
    }
    states.push(std::move(state));
}

State* Game::getCurrentState() {
    return states.empty() ? nullptr : states.top().get();
}

sf::RenderWindow& Game::getWindow() {
    return window;
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (auto* current = getCurrentState()) {
            current->handleEvent(event);
        }
    }
}

void Game::update(float dt) {
    if (auto* current = getCurrentState()) {
        current->update(dt);
    }
}

void Game::render() {
    window.clear();
    if (auto* current = getCurrentState()) {
        current->render(window);
    }
    window.display();
}

void Game::startBgm() {
    if (bgm.getStatus() == sf::Music::Stopped) {
        if (!bgm.openFromFile("assets/audio/Back to Business - William Benckert.mp3")) {
            return; // silently fail if file missing
        }
        bgm.setLoop(true);
        bgm.setVolume(20.f);
        bgm.play();
    } else if (bgm.getStatus() == sf::Music::Paused) {
        bgm.play(); // resume from last position
    }
}

void Game::pauseBgm() {
    if (bgm.getStatus() == sf::Music::Playing) {
        bgm.pause(); // pause, keeping position
    }
}

void Game::resumeBgm() {
    if (bgm.getStatus() == sf::Music::Paused) {
        bgm.play(); // resume from paused position
    }
}