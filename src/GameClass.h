//
// Created by RicardoLocher on 05.08.2024.
//

#ifndef GAMECLASS_H
#define GAMECLASS_H

#include "iostream"

#include "SFML/Graphics.hpp"

#include "PlayerClass.h"

class Game {
public:
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML ROCKS!!!"), player(), font(), fpsText() {
        if (!font.loadFromFile("../../assets/manolomono.otf")) {
            std::cout << "Error Loading Font";
            return;
        }

        fps = 0;
        frameCount = 0;
        fpsText.setFont(font);
        fpsText.setCharacterSize(24);
        fpsText.setFillColor(sf::Color::White);
        fpsText.setPosition(10.f, 10.f);
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
                update(TimePerFrame);
            }
            render();
        }
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
    }

    void render() {
        window.clear();
        player.render(window);
        window.draw(fpsText);
        window.display();
    }

    sf::RenderWindow window;

    Player player;

    sf::Time TimePerFrame = sf::seconds(1.f / 144.f);

    sf::Font font;
    sf::Text fpsText;
    sf::Clock fpsClock;
    int frameCount;
    float fps;
};

#endif //GAMECLASS_H
