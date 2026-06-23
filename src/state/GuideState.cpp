#include "GuideState.h"
#include "Game.h"
#include "MenuState.h"
#include <iostream>

GuideState::GuideState(Game& game) : State(game) {
    if (!guideTexture.loadFromFile("assets/image/guide.png")) {
        throw std::runtime_error("Gagal memuat gambar panduan!");
    }
    
    guideSprite.setTexture(guideTexture);
    guideSprite.setScale(
        static_cast<float>(WINDOW_WIDTH) / guideTexture.getSize().x,
        static_cast<float>(WINDOW_HEIGHT) / guideTexture.getSize().y
    );
}

void GuideState::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        std::cout << "ESC pressed in GuideState, returning to Menu\n";
        game.changeState(std::make_unique<MenuState>(game));
    }
}

void GuideState::update(float dt) {
    // Tidak perlu update untuk state statis
}

void GuideState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(guideSprite);
    window.display();
}
