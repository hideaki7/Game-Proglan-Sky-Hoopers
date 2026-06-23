#include "MenuState.h"
#include "GameState.h"
#include "Game.h"
#include "GuideState.h"
#include <functional>

MenuState::MenuState(Game& game) : State(game) {
    // Background
    if (!bgTexture.loadFromFile("assets/image/background/bgMenu.png")) {
        throw std::runtime_error("Failed to load bgMenu.png");
    }
    bgTexture.setRepeated(true);
    bgSprite.setTexture(bgTexture);

    // Scale background to fill the window
    float bgScaleX = static_cast<float>(WINDOW_WIDTH) / bgTexture.getSize().x;
    float bgScaleY = static_cast<float>(WINDOW_HEIGHT) / bgTexture.getSize().y;
    float bgScale  = std::max(bgScaleX, bgScaleY);
    bgSprite.setScale(bgScale, bgScale);
    // Center the background
    bgSprite.setPosition(
        (WINDOW_WIDTH  - bgTexture.getSize().x * bgScale) / 2.f,
        (WINDOW_HEIGHT - bgTexture.getSize().y * bgScale) / 2.f
    );

    //Sign / Logo
    if (!signTexture.loadFromFile("assets/image/sign.png")) {
        throw std::runtime_error("Failed to load sign.png");
    }
    signSprite.setTexture(signTexture);

    // Scale sign to ~72% of window width while keeping aspect ratio
    float signTargetW = WINDOW_WIDTH * 0.72f;
    float signScale   = signTargetW / signTexture.getSize().x;
    signSprite.setScale(signScale, signScale);
    // Center horizontally, place near the top (~8% from top)
    signSprite.setPosition(
        (WINDOW_WIDTH - signTexture.getSize().x * signScale) / 2.f,
        WINDOW_HEIGHT * 0.08f
    );

    //Button texture & font
    if (!btnTexture.loadFromFile("assets/image/menu/button.png")) {
        throw std::runtime_error("Failed to load button.png");
    }

    if (!arcadeFont.loadFromFile("assets/font/ARCADE_N.TTF")) {
        throw std::runtime_error("Failed to load ARCADE_N.TTF");
    }

    setupButtons();

    // Start BGM on launch / menu
    game.startBgm();
}

void MenuState::update(float dt) {
    // Update hover state based on current mouse position
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));

    hoveredIndex = -1;
    for (int i = 0; i < static_cast<int>(buttons.size()); ++i) {
        if (buttons[i].area.contains(mousePos)) {
            hoveredIndex = i;
            break;
        }
    }
}

void MenuState::setupButtons() {
    // Button layout constants
    const float btnW   = 230.f;   // desired button width on screen
    const float btnH   = 60.f;    // desired button height on screen
    const float startY = WINDOW_HEIGHT * 0.52f;  // first button Y
    const float gap    = 80.f;                    // spacing between buttons
    const float centerX = WINDOW_WIDTH / 2.f;

    // Scale factor from the tiny 48×16 texture to the desired size
    float btnScaleX = btnW / btnTexture.getSize().x;
    float btnScaleY = btnH / btnTexture.getSize().y;

    // Button definitions: label, action
    struct ButtonDef {
        std::string label;
        std::function<void()> action;
    };

    std::vector<ButtonDef> defs = {
        {"START", [this]() {
            game.changeState(std::make_unique<GameState>(game));
        }},
        {"GUIDE", [this]() {
            game.changeState(std::make_unique<GuideState>(game));
        }},
        {"EXIT", [this]() {
            game.getWindow().close();
        }}
    };

    for (int i = 0; i < static_cast<int>(defs.size()); ++i) {
        MenuButton btn;

        float x = centerX - btnW / 2.f;
        float y = startY + i * gap;

        // Sprite
        btn.sprite.setTexture(btnTexture);
        btn.sprite.setScale(btnScaleX, btnScaleY);
        btn.sprite.setPosition(x, y);

        // Label
        btn.label.setFont(arcadeFont);
        btn.label.setString(defs[i].label);
        btn.label.setCharacterSize(22);
        btn.label.setFillColor(sf::Color::White);

        // Center text on the button
        sf::FloatRect textBounds = btn.label.getLocalBounds();
        btn.label.setOrigin(
            textBounds.left + textBounds.width / 2.f,
            textBounds.top  + textBounds.height / 2.f
        );
        btn.label.setPosition(
            x + btnW / 2.f,
            y + btnH / 2.f - 6.f
        );

        // Clickable area
        btn.area = sf::FloatRect(x, y, btnW, btnH);

        // Action
        btn.action = defs[i].action;

        buttons.push_back(std::move(btn));
    }
}

void MenuState::handleEvent(sf::Event& event) {
    // Tekan Escape untuk keluar
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        game.getWindow().close();
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
            sf::Mouse::getPosition(game.getWindow()));

        for (const auto& button : buttons) {
            if (button.area.contains(mousePos)) {
                button.action();
                break;
            }
        }
    }
}

void MenuState::render(sf::RenderWindow& window) {
    // 1. Background
    window.draw(bgSprite);

    // 2. Sign / logo
    window.draw(signSprite);

    // 3. Buttons
    for (int i = 0; i < static_cast<int>(buttons.size()); ++i) {
        const auto& btn = buttons[i];

        // Draw button sprite (with slight scale-up on hover)
        sf::Sprite drawSprite = btn.sprite;
        sf::Text   drawLabel  = btn.label;

        if (i == hoveredIndex) {
            // Slight enlarge effect on hover (scale up from center)
            float hoverScale = 1.08f;
            float origW = btn.area.width;
            float origH = btn.area.height;
            float newW  = origW * hoverScale;
            float newH  = origH * hoverScale;

            drawSprite.setScale(
                drawSprite.getScale().x * hoverScale,
                drawSprite.getScale().y * hoverScale
            );
            drawSprite.setPosition(
                btn.area.left - (newW - origW) / 2.f,
                btn.area.top  - (newH - origH) / 2.f
            );

            // Slightly larger text
            drawLabel.setCharacterSize(static_cast<unsigned int>(22 * hoverScale));
            sf::FloatRect tb = drawLabel.getLocalBounds();
            drawLabel.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            drawLabel.setPosition(
                btn.area.left + origW / 2.f,
                btn.area.top  + origH / 2.f - 6.f * hoverScale
            );
        }

        window.draw(drawSprite);
        window.draw(drawLabel);
    }
}