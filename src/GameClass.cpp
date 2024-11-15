//
// Created by RicardoLocher on 08.10.2024.
//

#include <iostream>
#include <random>

#include "GameClass.h"
#include "Gui/Starbackground.h"
#include "Gui/StartMenu.h"

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "ASTEROIDS"), player(WINDOW_WIDTH, WINDOW_HEIGHT), starBackground(window, 100), font(), fpsText()  {

    startMenu(window);

    if (!font.loadFromFile("../../assets/manolomono.otf")) {
        std::cout << "Error Loading Font \n";
        return;
    }
    if (!asteroid1.loadFromFile("../../assets/meteor.png")) {
        std::cout << "Error Loading meteor.png \n";
    }
    if (!asteroid2.loadFromFile("../../assets/meteor2.png")) {
        std::cout << "Error Loading meteor2.png \n";
    }

    if (!asteroidLargeBuffer.loadFromFile("../../assets/Asteroid-hit-large.wav")) {
        std::cout << "Error Loading Asteroid Hit Sound Large \n";
    }
    asteroidLargeSound.setBuffer(asteroidLargeBuffer);
    asteroidLargeSound.setVolume(20.0f);

    if (!asteroidSmallBuffer.loadFromFile("../../assets/Asteroid-hit-small.wav")) {
        std::cout << "Error Loading Asteroid Hit Sound Small \n";
    }
    asteroidSmallSound.setBuffer(asteroidSmallBuffer);
    asteroidSmallSound.setVolume(20.0f);

    fps = 0;
    frameCount = 0;
    fpsText.setFont(font);
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(10.f, 10.f);

    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(10.f, 40.f);

    pointsText.setFont(font);
    pointsText.setCharacterSize(24);
    pointsText.setFillColor(sf::Color::White);
    pointsText.setPosition(10.f, 70.f);

    gameOver = false;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nPress 'R' to restart!");
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.width / 2, textRect.height / 2);
    gameOverText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // NOLINT(*-narrowing-conversions, *-integer-division)

    generateAsteroids(player.playerLevel);
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (window.isOpen()) {
        processEvents();
        if (!gameOver) {
            timeSinceLastUpdate += clock.restart();
            while (timeSinceLastUpdate > TimePerFrame) {
                timeSinceLastUpdate -= TimePerFrame;
                processEvents();
                checkCollisions();
                update(TimePerFrame);
            }
        }
        render();
    }
}

// Game cycle logic
void Game::processEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        // ReSharper disable once CppIncompleteSwitchStatement
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (gameOver && event.key.code == sf::Keyboard::R) {
                    resetGame();
                }
                else if(event.key.code == sf::Keyboard::Escape) {
                    startMenu(window);
                }
                else {
                    player.handleInput(event.key.code, true);
                }
            break;

            case sf::Event::Resized:
                starBackground.updateOnResize();

            case sf::Event::KeyReleased:
                player.handleInput(event.key.code, false);
            break;

            case sf::Event::Closed:
                window.close();
        }
    }
}
void Game::update(sf::Time deltaTime) {
    frameCount++;
    if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        fps = frameCount / fpsClock.getElapsedTime().asSeconds(); // NOLINT(*-narrowing-conversions)
        fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
        frameCount = 0;
        fpsClock.restart();
    }

    levelText.setString("LEVEL: " + std::to_string(player.playerLevel));
    pointsText.setString("Points: " + std::to_string(player.points));

    player.update(deltaTime, WINDOW_WIDTH, WINDOW_HEIGHT);
    updateAsteroids(deltaTime,  WINDOW_WIDTH, WINDOW_HEIGHT);
}
void Game::render() {
    window.clear();
    starBackground.draw(window);
    if  (gameOver) {
        window.draw(gameOverText);
    } else {
        player.render(window);
        window.draw(fpsText);
        window.draw(levelText);
        window.draw(pointsText);
        for (const auto& asteroid : asteroids) {
            asteroid.render(window);
        }
    }
    window.display();
}

// Asteroids logic (will soon be refactored and moved to another class)
void Game::generateAsteroids(int level) {
    const sf::Vector2f playerPos = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f}; // NOLINT(*-narrowing-conversions)
    // ReSharper disable once CppVariableCanBeMadeConstexpr
    // ReSharper disable once CppTooWideScope
    const float minDistance = 400.0f; // Minimum distance from the player
    for (int i = 1; i < (level + 6); i++) {
        asteroids.emplace_back(generateAsteroid_Pos(playerPos, minDistance), asteroid1, generateAsteroid_Direction(), false);
    }
}
void Game::generateMiniAsteroids(sf::Vector2f pos) {
    for (int i = 0; i < 2; i++) {
        asteroids.emplace_back(pos, asteroid2, generateAsteroid_Direction(), true);
    }
}
sf::Vector2f Game::generateAsteroid_Pos(const sf::Vector2f& playerPos, float minDistance) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr1(0, 1920);
    std::uniform_int_distribution<> distr2(0, 1080);

    sf::Vector2f asteroidPos;
    do {
        const int num1 = distr1(gen);
        const int num2 = distr2(gen);
        asteroidPos = {static_cast<float>(num1), static_cast<float>(num2)};
    } while (std::sqrt(std::pow(asteroidPos.x - playerPos.x, 2) + std::pow(asteroidPos.y - playerPos.y, 2)) < minDistance);

    return asteroidPos;
}
float Game::generateAsteroid_Direction() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 360);
    const int num = distr(gen);
    return static_cast<float>(num);
}
void Game::updateAsteroids(const sf::Time deltaTime, const int WindowWidth, const int WindowHeight) {
    // ReSharper restore CppDFAConstantParameter
    for (auto i = asteroids.begin(); i != asteroids.end(); ) {
        i->update(deltaTime, WINDOW_WIDTH, WINDOW_HEIGHT);
        sf::Vector2f pos = i->getPosition();
        if (pos.x < 0.f || pos.x > WindowWidth || pos.y < 0.f || pos.y > WindowHeight) { // NOLINT(*-narrowing-conversions)
            i = asteroids.erase(i);
        } else {
            ++i;
        }
    }
    if (asteroids.empty()) {
        nextLevel();
    }
}

// Collision logic (will soon be moved from this class)
void Game::checkCollisions() {
    for (const auto& asteroid : asteroids) {
        if (pixelPerfectCollision(player.getSprite(), player.getImage(), asteroid.getSprite(), asteroid.getImage())) {
            gameOver = true;
            return;
        }
    }

    // Check collisions between lasers and asteroids
    auto& lasers = player.getLasers();
    for (auto laserIt = lasers.begin(); laserIt != lasers.end(); ) {
        bool laserHit = false;
        for (auto asteroidIt = asteroids.begin(); asteroidIt != asteroids.end(); ) {
            if (pixelPerfectCollision(laserIt->getSprite(), player.getImage(), asteroidIt->getSprite(), asteroidIt->getImage())) {
                // Handle collision (e.g., destroy asteroid, remove laser)
                // ReSharper disable once CppDFAUnusedValue
                if (asteroidIt -> isMini) {
                    asteroidSmallSound.play();
                    asteroidIt = asteroids.erase(asteroidIt);
                    player.points += 10;
                }
                else {
                    asteroidLargeSound.play();
                    sf::Vector2f pos = asteroidIt -> getPosition();
                    asteroidIt = asteroids.erase(asteroidIt);
                    generateMiniAsteroids(pos);
                    player.points += 10;
                }
                laserHit = true;
                break;
            }
            ++asteroidIt;
        }
        if (laserHit) {
            laserIt = lasers.erase(laserIt);
            player.hasActiveLaser = false;
        } else {
            ++laserIt;
        }
    }
}
bool Game::pixelPerfectCollision(const sf::Sprite& sprite1, const sf::Image& image1, const sf::Sprite& sprite2, const sf::Image& image2) {
    sf::FloatRect bounds1 = sprite1.getGlobalBounds();
    sf::FloatRect bounds2 = sprite2.getGlobalBounds();

    if (!bounds1.intersects(bounds2)) {
        return false;
    }

    sf::FloatRect intersection;
    if (bounds1.intersects(bounds2, intersection)) {
        for (int x = intersection.left; x < intersection.left + intersection.width; ++x) { // NOLINT(*-narrowing-conversions)
            for (int y = intersection.top; y < intersection.top + intersection.height; ++y) { // NOLINT(*-narrowing-conversions)
                // ReSharper disable CppTooWideScopeInitStatement
                const sf::Vector2f point1 = sprite1.getInverseTransform().transformPoint(static_cast<float>(x), static_cast<float>(y));
                const sf::Vector2f point2 = sprite2.getInverseTransform().transformPoint(static_cast<float>(x), static_cast<float>(y));
                // ReSharper restore CppTooWideScopeInitStatement

                if (point1.x >= 0 && point1.x < image1.getSize().x && // NOLINT(*-narrowing-conversions)
                    point1.y >= 0 && point1.y < image1.getSize().y && // NOLINT(*-narrowing-conversions)
                    point2.x >= 0 && point2.x < image2.getSize().x && // NOLINT(*-narrowing-conversions)
                    point2.y >= 0 && point2.y < image2.getSize().y) { // NOLINT(*-narrowing-conversions)

                    if (image1.getPixel(static_cast<unsigned int>(point1.x), static_cast<unsigned int>(point1.y)).a != 0 &&
                        image2.getPixel(static_cast<unsigned int>(point2.x), static_cast<unsigned int>(point2.y)).a != 0) {
                        return true;
                        }
                    }
            }
        }
    }
    return false;
}

// Game flow control
void Game::resetGame() {
    player.reset(WINDOW_WIDTH, WINDOW_HEIGHT);
    asteroids.clear();
    generateAsteroids(player.playerLevel);
    gameOver = false;
    frameCount = 0;
    fpsClock.restart();
}
void Game::nextLevel() {
    player.playerLevel += 1;
    generateAsteroids(player.playerLevel);
}

void Game::startMenu(sf::RenderWindow& window) {
    StartMenu menu(window);
    int menuSelection = menu.displayMenu();
    if (menuSelection == 0) {
        resetGame();
    }
    else {
        window.close();
    }
}
