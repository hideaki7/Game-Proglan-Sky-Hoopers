#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "entities/Portal.h"
#include "entities/Player.h"
#include "entities/Platform.h"
#include "entities/Buff.h" // Include untuk DoublePointBuff, RocketBuff, ShieldBuff
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

class GameState : public State {
public:
    explicit GameState(Game& game);
    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    Player player;
    std::vector<std::unique_ptr<Buff>> buffs;
    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<Portal> portals;
    
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text restartText;
    
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::Texture bgTexture2;  // Background horizontal mode
    sf::Sprite bgSprite2;
    float bgFadeAlpha = 0.f;      // 0 = bgGame, 1 = bgGame2
    float targetBgAlpha = 0.f;    // Target fade

    float buffSpawnTimer = 0.f;
    const float BUFF_SPAWN_INTERVAL = 2.f; // Lebih sering spawn buff (tiap 2 detik)
    int score = 0;
    bool isGameOver = false;
    bool isPaused = false;
    GravityMode currentGravity = GravityMode::Vertical;
    GravityMode currentPortalMode = GravityMode::Horizontal;
    

    void spawnInitialPlatforms();
    void spawnNewPlatforms();
    void spawnNewPlatformsHorizontal();
    void checkCollisions();
    void updateScroll(float dt);

    sf::Text pauseText;
    sf::Text pauseSubText;
    sf::Text resumeBtn;
    sf::Text exitBtn;
    sf::Vector2f mousePos;

    // Audio
    sf::SoundBuffer gameOverBuffer;
    sf::Sound gameOverSound;
    sf::SoundBuffer portalBuffer;
    sf::Sound portalSound;
};

#endif