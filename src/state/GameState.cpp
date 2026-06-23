#include "GameState.h"
#include "Game.h"
#include "state/MenuState.h"
#include "entities/DoublePointBuff.h"
#include "entities/RocketBuff.h"
#include "entities/ShieldBuff.h"
#include <algorithm>
#include <memory>
#include <sstream>

GameState::GameState(Game& game) : State(game),

      player(),
      score(0),
      isGameOver(false),
      isPaused(false),
      currentGravity(GravityMode::Vertical),
      buffSpawnTimer(0.f)
{
    
    if (!font.loadFromFile("assets/font/ARCADE_N.TTF")) {
        throw std::runtime_error("Failed to load font!");
    }
    
    if (!bgTexture.loadFromFile("assets/image/background/bgGame.png")) {
        throw std::runtime_error("Failed to load game background");
    }
    bgSprite.setTexture(bgTexture);
    
    float scale = std::max(
        (float)WINDOW_WIDTH / bgSprite.getLocalBounds().width,
        (float)WINDOW_HEIGHT / bgSprite.getLocalBounds().height
    );
    bgSprite.setScale(scale, scale);
    bgSprite.setPosition(
        (WINDOW_WIDTH - bgSprite.getGlobalBounds().width) / 2.f,
        (WINDOW_HEIGHT - bgSprite.getGlobalBounds().height) / 2.f
    );

    // Background untuk horizontal mode
    if (!bgTexture2.loadFromFile("assets/image/background/bgGame2.png")) {
        throw std::runtime_error("Failed to load game background 2");
    }
    bgSprite2.setTexture(bgTexture2);
    float scale2 = std::max(
        (float)WINDOW_WIDTH / bgSprite2.getLocalBounds().width,
        (float)WINDOW_HEIGHT / bgSprite2.getLocalBounds().height
    );
    bgSprite2.setScale(scale2, scale2);
    bgSprite2.setPosition(
        (WINDOW_WIDTH - bgSprite2.getGlobalBounds().width) / 2.f,
        (WINDOW_HEIGHT - bgSprite2.getGlobalBounds().height) / 2.f
    );
    bgSprite2.setColor(sf::Color(255, 255, 255, 0));

    DoublePointBuff::loadTexture();
    RocketBuff::loadTexture();
    ShieldBuff::loadTexture();
    Platform::loadTextures();
    Portal::loadTextures();

    DoublePointBuff::loadSfx();
    RocketBuff::loadSfx();
    ShieldBuff::loadSfx();

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 10);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(35);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(WINDOW_WIDTH/2 - 155, WINDOW_HEIGHT/2 - 30);

    restartText.setFont(font);
    restartText.setCharacterSize(14);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("CLICK TO RESTART");

    sf::FloatRect rtBounds = restartText.getLocalBounds();
    restartText.setPosition(
        WINDOW_WIDTH / 2.f - rtBounds.width / 2.f,
        WINDOW_HEIGHT / 2.f + 30
    );

    pauseText.setFont(font);
    pauseText.setCharacterSize(35);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("PAUSED");
    sf::FloatRect ptBounds = pauseText.getLocalBounds();
    pauseText.setPosition(WINDOW_WIDTH / 2.f - ptBounds.width / 2.f, WINDOW_HEIGHT / 2.f - 80);

    pauseSubText.setFont(font);
    pauseSubText.setCharacterSize(12);
    pauseSubText.setFillColor(sf::Color(180, 180, 180));
    pauseSubText.setString("Press ESC to resume");
    sf::FloatRect psBounds = pauseSubText.getLocalBounds();
    pauseSubText.setPosition(WINDOW_WIDTH / 2.f - psBounds.width / 2.f, WINDOW_HEIGHT / 2.f - 30);

    resumeBtn.setFont(font);
    resumeBtn.setCharacterSize(18);
    resumeBtn.setFillColor(sf::Color::White);
    resumeBtn.setString("RESUME");
    sf::FloatRect rbBounds = resumeBtn.getLocalBounds();
    resumeBtn.setPosition(WINDOW_WIDTH / 2.f - rbBounds.width / 2.f, WINDOW_HEIGHT / 2.f + 10);

    exitBtn.setFont(font);
    exitBtn.setCharacterSize(18);
    exitBtn.setFillColor(sf::Color::White);
    exitBtn.setString("EXIT GAME");
    sf::FloatRect ebBounds = exitBtn.getLocalBounds();
    exitBtn.setPosition(WINDOW_WIDTH / 2.f - ebBounds.width / 2.f, WINDOW_HEIGHT / 2.f + 50);

    player.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    player.setGrounded(true);

    spawnInitialPlatforms();

    currentPortalMode = GravityMode::Horizontal;
    portals.clear();
    float portalX = rand() % (WINDOW_WIDTH - 50);
    float portalY = player.getPosition().y - 200;
    portals.push_back(Portal(portalX, portalY, currentPortalMode));

    gameOverBuffer.loadFromFile("assets/audio/game over.mp3");
    gameOverSound.setBuffer(gameOverBuffer);
    gameOverSound.setVolume(80.f);

    portalBuffer.loadFromFile("assets/audio/portalsfx.mp3");
    portalSound.setBuffer(portalBuffer);
    portalSound.setVolume(100.f);

    game.startBgm();
}

void GameState::spawnInitialPlatforms() {
    const float startY = WINDOW_HEIGHT - 50.f;

    platforms.push_back(std::make_unique<Platform>(
        WINDOW_WIDTH / 2 - 50.f, 
        startY, 
        Platform::NORMAL
    ));

    for (int i = 1; i < 12; ++i) {
        float x = static_cast<float>(rand() % (WINDOW_WIDTH - 100));
        float y = static_cast<float>(WINDOW_HEIGHT - (i * 110));
        int type = rand() % 10;
        
        if (type <= 5) {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::NORMAL));
        }
        else if (type <= 8) {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::CRACKED));
        }
        else {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::SPIKE));
        }
    }
}

void GameState::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
        mousePos = game.getWindow().mapPixelToCoords(pixelPos);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (isGameOver) {

            game.changeState(std::make_unique<MenuState>(game));
            return;
        }

        isPaused = !isPaused;
        return;
    }

    if (isGameOver && event.type == sf::Event::MouseButtonPressed) {
        game.resumeBgm();
        game.changeState(std::make_unique<GameState>(game));
        return;
    }

    if (isPaused && event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f click = game.getWindow().mapPixelToCoords(pixelPos);
        if (resumeBtn.getGlobalBounds().contains(click)) {
            isPaused = false;
            return;
        }
        if (exitBtn.getGlobalBounds().contains(click)) {
            game.pauseBgm();
            game.changeState(std::make_unique<MenuState>(game));
            return;
        }
    }

    if (isPaused || isGameOver) return;

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            player.move(-1.0f);
        } 
        else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            player.move(1.0f);
        }
    }
    
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D || 
            event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
            player.move(0);
        }
    }
}

void GameState::update(float dt) {
    if (isGameOver || isPaused) return;

    // Animasi fade background
    float fadeSpeed = 2.0f; // kecepatan transisi (detik)
    if (bgFadeAlpha < targetBgAlpha) {
        bgFadeAlpha += dt * fadeSpeed;
        if (bgFadeAlpha > targetBgAlpha) bgFadeAlpha = targetBgAlpha;
    } else if (bgFadeAlpha > targetBgAlpha) {
        bgFadeAlpha -= dt * fadeSpeed;
        if (bgFadeAlpha < targetBgAlpha) bgFadeAlpha = targetBgAlpha;
    }
    // Update alpha pada sprite
    sf::Uint8 alpha2 = static_cast<sf::Uint8>(bgFadeAlpha * 255.f);
    sf::Uint8 alpha1 = 255 - alpha2;
    bgSprite.setColor(sf::Color(255, 255, 255, alpha1));
    bgSprite2.setColor(sf::Color(255, 255, 255, alpha2));

    buffSpawnTimer += dt;
    if (buffSpawnTimer >= BUFF_SPAWN_INTERVAL) {
        buffSpawnTimer = 0.f;
        float x = static_cast<float>(rand() % (WINDOW_WIDTH - 32));
        float y = static_cast<float>(rand() % (WINDOW_HEIGHT - 200));
        
        int type = rand() % 3;
        if (type == 0) {
            buffs.push_back(std::make_unique<DoublePointBuff>(x, y));
        }
        else if (type == 1) {
            buffs.push_back(std::make_unique<RocketBuff>(x, y));
        }
        else {
            buffs.push_back(std::make_unique<ShieldBuff>(x, y));
        }
    }

    player.update(dt);

    checkCollisions();

    // Hapus buff yang sudah dipakai
    buffs.erase(
        std::remove_if(buffs.begin(), buffs.end(),
            [](const std::unique_ptr<Buff>& b) { return b->isUsed(); }),
        buffs.end());

    updateScroll(dt);

    // Hapus platform yang sudah keluar layar sesuai gravity mode
    if (currentGravity == GravityMode::Vertical) {
        // Vertikal: hapus yang keluar bawah
        platforms.erase(
            std::remove_if(platforms.begin(), platforms.end(), 
                [](const std::unique_ptr<Platform>& p) { 
                    return p->getY() > WINDOW_HEIGHT + 50; 
                }),
            platforms.end());
    } else {
        // Horizontal: hapus yang keluar kiri
        platforms.erase(
            std::remove_if(platforms.begin(), platforms.end(), 
                [](const std::unique_ptr<Platform>& p) { 
                    return p->getX() < -PLATFORM_WIDTH - 50; 
                }),
            platforms.end());
    }

    // Spawn platform baru jika kurang dari 10
    if (static_cast<int>(platforms.size()) < 10) {
        if (currentGravity == GravityMode::Vertical) {
            spawnNewPlatforms();
        } else {
            spawnNewPlatformsHorizontal();
        }
    }

    std::ostringstream ss;
    ss << "Score: " << player.getScore();
    scoreText.setString(ss.str());

    // Game over check
    if (player.isDead()) {
        if (!isGameOver) {
            isGameOver = true;
            game.pauseBgm();
            gameOverSound.play();
        }
    } else if (currentGravity == GravityMode::Vertical) {
        // Vertikal: jatuh ke bawah layar
        if (player.getPosition().y > WINDOW_HEIGHT + 50) {
            if (!isGameOver) {
                isGameOver = true;
                game.pauseBgm();
                gameOverSound.play();
            }
        }
    } else {
        // Horizontal: keluar kiri layar ATAU jatuh ke bawah layar
        if (player.getPosition().x < -50 || player.getPosition().y > WINDOW_HEIGHT + 50) {
            if (!isGameOver) {
                isGameOver = true;
                game.pauseBgm();
                gameOverSound.play();
            }
        }
    }
}

void GameState::checkCollisions() {
    player.setGrounded(false);

    bool landed = false;
    for (auto& platform : platforms) {
        if (!player.isRocketing() && !landed && platform->checkCollision(player.getBounds(), currentGravity)) {
            platform->onCollision(player);
            landed = true;

            // Skor hanya bertambah jika platform baru (belum pernah diinjak)
            int pid = platform->getId();
            if (!player.hasSteppedOn(pid)) {
                player.markStepped(pid);
                if (platform->getType() == Platform::NORMAL) {
                    player.addScore(10);
                } else if (platform->getType() == Platform::CRACKED) {
                    player.addScore(20);
                }
            }
        }
    }

    for (auto& buff : buffs) {
        if (buff->checkCollision(player.getBounds())) {
            buff->apply(player);
        }
    }

    for (auto& portal : portals) {
        if (portal.checkCollision(player.getBounds()) && player.canTeleport()) {
            GravityMode newMode = portal.getTargetMode();
            if (newMode != currentGravity) {
                currentGravity = newMode;
                player.switchGravityMode(currentGravity);
                player.markTeleported();

                // Play portal SFX — stop dulu lalu play ulang agar selalu berbunyi
                portalSound.stop();
                portalSound.play();

                // Ganti portal mode untuk spawn berikutnya
                currentPortalMode = (currentPortalMode == GravityMode::Vertical)
                                    ? GravityMode::Horizontal
                                    : GravityMode::Vertical;

                portals.clear();

                // Reset platforms dan stepped tracking untuk mode baru
                platforms.clear();
                player.clearSteppedPlatforms();
                if (currentGravity == GravityMode::Horizontal) {
                    // Masuk horizontal: spawn platform di sekitar player ke kanan
                    targetBgAlpha = 1.f; // fade ke bgGame2
                    float baseX = player.getPosition().x;
                    for (int i = 0; i < 10; ++i) {
                        float px = baseX + static_cast<float>(rand() % 70 + 60) * (i + 1);
                        float py = static_cast<float>(rand() % (WINDOW_HEIGHT - 200) + 100);
                        platforms.push_back(std::make_unique<Platform>(px, py, Platform::NORMAL));
                    }
                    // Tambah platform di bawah player sebagai pijakan
                    platforms.push_back(std::make_unique<Platform>(
                        player.getPosition().x - 50, 
                        player.getPosition().y + 60, 
                        Platform::NORMAL));
                } else {
                    // Kembali ke vertikal: spawn platform di bawah dan atas player
                    targetBgAlpha = 0.f; // fade kembali ke bgGame
                    float baseY = player.getPosition().y;
                    // Platform pijakan langsung di bawah player
                    platforms.push_back(std::make_unique<Platform>(
                        player.getPosition().x - 50, 
                        baseY + 60, 
                        Platform::NORMAL));
                    for (int i = 1; i < 12; ++i) {
                        float px = static_cast<float>(rand() % (WINDOW_WIDTH - 100));
                        float py = baseY - static_cast<float>(i * 110);
                        platforms.push_back(std::make_unique<Platform>(px, py, Platform::NORMAL));
                    }
                }

                float newX, newY;
                if (currentGravity == GravityMode::Vertical) {
                    newX = rand() % (WINDOW_WIDTH - 50);
                    newY = player.getPosition().y - 600;
                } else {
                    newX = player.getPosition().x + 600;
                    newY = rand() % (WINDOW_HEIGHT - 100) + 50;
                }
                portals.push_back(Portal(newX, newY, currentPortalMode));
            }
        }
    }
}

void GameState::updateScroll(float dt) {
    if (currentGravity == GravityMode::Vertical) {
        // Pure tracking camera: kamera SELALU mengikuti posisi Y player
        // Target: player berada di 60% dari atas layar
        float targetScreenY = WINDOW_HEIGHT * 0.6f;
        float playerY = player.getPosition().y;

        // Hanya scroll ke atas (saat player di atas target)
        if (playerY < targetScreenY) {
            float dy = targetScreenY - playerY;
            
            player.setPosition(player.getPosition().x, targetScreenY);
            
            for (auto& p : platforms) {
                p->update(0, dy);
            }
            
            for (auto& portal : portals) {
                portal.update(0, dy);
            }
            
            for (auto& buff : buffs) {
                buff->update(0, dy);
            }

            // Respawn portal jika sudah keluar layar bawah
            portals.erase(
                std::remove_if(portals.begin(), portals.end(),
                    [](const Portal& p) { return p.getPosition().y > WINDOW_HEIGHT + 50; }),
                portals.end());

            if (portals.empty()) {
                currentPortalMode = GravityMode::Horizontal;
                float newX = rand() % (WINDOW_WIDTH - 50);
                float newY = -50.f;
                portals.push_back(Portal(newX, newY, currentPortalMode));
            }
        }
    } else {
        // Scroll horizontal: kamera ikut player ke KANAN
        int hScrollThreshold = WINDOW_WIDTH / 2;
        if (player.getPosition().x > hScrollThreshold) {
            float dx = hScrollThreshold - player.getPosition().x; // negatif → push ke kiri
            
            player.setPosition(hScrollThreshold, player.getPosition().y);
            
            for (auto& p : platforms) {
                p->update(dx, 0);
            }
            
            for (auto& portal : portals) {
                portal.update(dx, 0);
            }
            
            for (auto& buff : buffs) {
                buff->update(dx, 0);
            }

            // Respawn portal jika sudah keluar layar kiri
            portals.erase(
                std::remove_if(portals.begin(), portals.end(),
                    [](const Portal& p) { return p.getPosition().x < -50; }),
                portals.end());

            if (portals.empty()) {
                currentPortalMode = GravityMode::Vertical;
                float newX = WINDOW_WIDTH + 50.f;
                float newY = rand() % (WINDOW_HEIGHT - 100) + 50;
                portals.push_back(Portal(newX, newY, currentPortalMode));
            }
        }
    }
}

void GameState::spawnNewPlatforms() {
    float highestPlatform = WINDOW_HEIGHT;
    for (const auto& p : platforms) {
        if (p->getY() < highestPlatform) {
            highestPlatform = p->getY();
        }
    }

    for (int i = 0; i < 5; ++i) {
        float x = static_cast<float>(rand() % (WINDOW_WIDTH - 100));
        float y = highestPlatform - static_cast<float>(rand() % 80 + 80);
        int type = rand() % 10;
        
        if (type <= 6) {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::NORMAL));
        }
        else if (type <= 9) {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::CRACKED));
        }
        else {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::SPIKE));
        }

        highestPlatform = y;
    }
}

void GameState::spawnNewPlatformsHorizontal() {
    // Cari platform paling kanan
    float rightmostX = 0.f;
    if (!platforms.empty()) {
        rightmostX = platforms[0]->getX();
        for (const auto& p : platforms) {
            if (p->getX() > rightmostX) {
                rightmostX = p->getX();
            }
        }
    }

    for (int i = 0; i < 7; ++i) {
        // Spawn platform ke kanan, dengan jarak dekat dan posisi Y random
        float x = rightmostX + static_cast<float>(rand() % 70 + 60);
        float y = static_cast<float>(rand() % (WINDOW_HEIGHT - 150) + 50);
        int type = rand() % 10;
        
        if (type <= 6) {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::NORMAL));
        }
        else if (type <= 9) {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::CRACKED));
        }
        else {
            platforms.push_back(std::make_unique<Platform>(x, y, Platform::SPIKE));
        }

        rightmostX = x;
    }
}

void GameState::render(sf::RenderWindow& window) {
    // Draw both backgrounds with fade
    window.draw(bgSprite);
    window.draw(bgSprite2);

    // Draw platforms
    for (auto& platform : platforms) {
        platform->draw(window);
    }

    // Draw portals 
    for (auto& portal : portals) {
        portal.draw(window);
    }

    // Draw buffs 
    for (auto& buff : buffs) {
        buff->draw(window);
    }

    // Draw player
    player.draw(window);

    // Draw UI
    window.draw(scoreText);
    
    if (isGameOver) {
        // Semi-transparent overlay
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        window.draw(gameOverText);
        window.draw(restartText);
    }
    
    // Draw pause overlay
    if (isPaused) {
        // Semi-transparent overlay
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);
        window.draw(pauseText);
        window.draw(pauseSubText);

        // Hover effect untuk tombol Resume
        if (resumeBtn.getGlobalBounds().contains(mousePos)) {
            resumeBtn.setFillColor(sf::Color::Yellow);
        } else {
            resumeBtn.setFillColor(sf::Color::White);
        }
        window.draw(resumeBtn);

        // Hover effect untuk tombol Exit
        if (exitBtn.getGlobalBounds().contains(mousePos)) {
            exitBtn.setFillColor(sf::Color(255, 100, 100));
        } else {
            exitBtn.setFillColor(sf::Color::White);
        }
        window.draw(exitBtn);
    }
}