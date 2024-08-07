//
// Created by RicardoLocher on 05.08.2024.
//

#ifndef GAMECLASS_H
#define GAMECLASS_H

#include "iostream"
#include "random"

#include "SFML/Graphics.hpp"

#include "PlayerClass.h"
#include "AsteroidClass.h"

class Game {
public:
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML ROCKS!!!"), player(WINDOW_WIDTH, WINDOW_HEIGHT), font(), fpsText() {
        if (!font.loadFromFile("../../assets/manolomono.otf")) {
            std::cout << "Error Loading Font \n";
            return;
        }
        if (!asteroid1.loadFromFile("../../assets/meteor.png")) {
            std::cout << "Error Loading meteor.png \n";
        }

        fps = 0;
        frameCount = 0;
        fpsText.setFont(font);
        fpsText.setCharacterSize(24);
        fpsText.setFillColor(sf::Color::White);
        fpsText.setPosition(10.f, 10.f);

        generateAsteroids(player.playerLevel);
    }

    void run() {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        while (window.isOpen()) {
            processEvents();
            timeSinceLastUpdate += clock.restart();
            while (timeSinceLastUpdate > TimePerFrame) {
                timeSinceLastUpdate -= TimePerFrame;
                processEvents();
                checkCollision();
                update(TimePerFrame);
            }
            render();
        }
    }

    void generateAsteroids(const int level) {
        switch (level) { // NOLINT(*-multiway-paths-covered)
            case 1:
                for (int i = 1; i < 5; i++) {
                    asteroids.emplace_back(generateAsteroid_Pos(), asteroid1, generateAsteroid_Direction());
                }
            default:
                for (int i = 0; i < 1; i++) {
                    asteroids.emplace_back(generateAsteroid_Pos(), asteroid1, generateAsteroid_Direction());
                }
        }
    }

    static sf::Vector2f generateAsteroid_Pos() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr1(0, 1920);
        std::uniform_int_distribution<> distr2(0, 1080);
        const int num1 = distr1(gen);
        const int num2 = distr2(gen);
        return {static_cast<float>(num1), static_cast<float>(num2)};
    }

    static float generateAsteroid_Direction() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 360);
        const int num = distr(gen);
        return static_cast<float>(num);
    }

private:
    void processEvents() {
        sf::Event event{};
        while (window.pollEvent(event)) {
            // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
            // ReSharper disable once CppIncompleteSwitchStatement
            switch (event.type) {
                case sf::Event::KeyPressed:
                    player.handleInput(event.key.code, true);
                    break;

                case sf::Event::KeyReleased:
                    player.handleInput(event.key.code, false);
                    break;

                case sf::Event::Closed:
                    window.close();
            }
        }
    }

    void update(const sf::Time deltaTime) {
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frameCount / fpsClock.getElapsedTime().asSeconds(); // NOLINT(*-narrowing-conversions)
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
            frameCount = 0;
            fpsClock.restart();
        }

        player.update(deltaTime, WINDOW_WIDTH, WINDOW_HEIGHT);
        updateAsteroids(deltaTime,  WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    void render() {
        window.clear();
        player.render(window);
        window.draw(fpsText);
        for (const auto& asteroid : asteroids) {
            asteroid.render(window);
        }
        window.display();
    }

    // ReSharper disable CppDFAConstantParameter
    void updateAsteroids(const sf::Time deltaTime, const int WindowWidth, const int WindowHeight) {
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
    }

    void checkCollision() const {
        sf::FloatRect playerBounds = player.getBounds();
        for (const auto& asteroid : asteroids) {
            sf::FloatRect asteroidBounds = asteroid.getBounds();
            if (playerBounds.intersects(asteroidBounds)) {
                std::cout << "Player got hit \n";
            }
        }
    }

    sf::RenderWindow window;

    Player player;

    sf::Time TimePerFrame = sf::seconds(1.f / 144.f);

    sf::Font font;
    sf::Text fpsText;
    sf::Clock fpsClock;
    int frameCount;
    float fps;

    std::vector<Asteroid> asteroids;
    sf::Texture asteroid1;
};

#endif //GAMECLASS_H
