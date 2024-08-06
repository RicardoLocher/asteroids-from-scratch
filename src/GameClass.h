//
// Created by ricar on 05.08.2024.
//

#ifndef GAMECLASS_H
#define GAMECLASS_H

#include "iostream"
#include "cmath"

#include "SFML/Graphics.hpp"

class Game {
public:

    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML ROCKS!!!"), playerSprite(){
        // Player Init
        if (!playerTexture.loadFromFile("../../assets/ship.png")) {
            // Handle Loading error
            std::cout << "Error loading Player Texture\n";
            return;
        }
        playerSprite.setTexture(playerTexture);
        playerSprite.setPosition(100, 100);
        pIsMovingForward = false;
        pIsMovingBackwards = false;
        pIsRotatingLeft = false;
        pIsRotatingRight = false;

        // Set the origin to the center of the sprite
        const sf::FloatRect bounds = playerSprite.getLocalBounds();
        playerSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);


        // Load AFont and initialize fpsText
        if (!font.loadFromFile("../../assets/manolomono.otf")) {
            std::cout << "Error Loading Font";
            return;
        }

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
                    handlePlayerInput(event.key.code, true);
                    break;

                case sf::Event::KeyReleased:
                    handlePlayerInput(event.key.code, false);
                    break;

                case sf::Event::Closed:
                    window.close();
            }
        }
    }

    void check_window_detection(sf::Vector2f &movement, sf::Vector2f &newPosition) const {
        // Check window boundaries and adjust the movement if necessary
        if (newPosition.x < 0.f) {
            movement.x = 0.f;
            newPosition.x = 0.f;
        }
        if (newPosition.x + (playerRadius * 2) > static_cast<float>(WINDOW_WIDTH)) {
            movement.x = 0.f;
            newPosition.x = static_cast<float>(WINDOW_WIDTH) - (playerRadius * 2);
        }
        if (newPosition.y < 0.f) {
            movement.y = 0.f;
            newPosition.y = 0.f;
        }
        if (newPosition.y + (playerRadius * 2) > static_cast<float>(WINDOW_HEIGHT)) {
            movement.y = 0.f;
            newPosition.y = static_cast<float>(WINDOW_HEIGHT) - (playerRadius * 2);
        }
    }

    void update(const sf::Time deltaTime) {

        // Calculate and show fps
        frameCount++;
        if(fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps  = frameCount / fpsClock.getElapsedTime().asSeconds();
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
            frameCount = 0;
            fpsClock.restart();
        }


        sf::Vector2f movement(0.f,0.f);

        // ReSharper disable once CppLocalVariableMayBeConst
        float rotation = playerSprite.getRotation();
        // ReSharper disable once CppLocalVariableMayBeConst
        float radian = 3.14159265 * (rotation / 180);

        sf::Vector2f forwardDirection(std::cos(radian), std::sin(radian));

        if (pIsMovingForward) movement += forwardDirection * movementSpeed;
        if (pIsMovingBackwards) movement -= forwardDirection * movementSpeed;

        // Basic collision detection with window borders
        // Get the player's new position based on the current movement
        sf::Vector2f newPosition = playerSprite.getPosition() + movement * deltaTime.asSeconds();

        check_window_detection(movement, newPosition);

        // Apply the corrected movement
        playerSprite.setPosition(newPosition);

        // Apply rotation
        if (pIsRotatingLeft) {
            playerSprite.rotate(-rotationSpeed * deltaTime.asSeconds());
        }
        if (pIsRotatingRight) {
            playerSprite.rotate(rotationSpeed * deltaTime.asSeconds());
        }
    }

    void render() {
        window.clear();
        window.draw(playerSprite);
        window.draw(fpsText);
        window.display();
    }

    void handlePlayerInput(const sf::Keyboard::Key key, const bool isPressed) {
        if (key == sf::Keyboard::W) pIsMovingForward = isPressed;
        else if (key == sf::Keyboard::S) pIsMovingBackwards = isPressed;
        else if (key == sf::Keyboard::D) pIsRotatingRight = isPressed;
        else if (key == sf::Keyboard::A) pIsRotatingLeft = isPressed;
    }

private:
    sf::RenderWindow window;

    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    sf::Time TimePerFrame = sf::seconds(1.f / 144.f);

    bool pIsMovingForward;
    bool pIsMovingBackwards;
    bool pIsRotatingRight;
    bool pIsRotatingLeft;

    float playerRadius = 10.f;
    float movementSpeed = 1000;
    float rotationSpeed = 200.f; // Rotation speed in degrees per second

    // All for the FPS Text:
    sf::Font font;
    sf::Text fpsText;
    sf::Clock fpsClock;
    int frameCount;
    float fps;
};

#endif //GAMECLASS_H
