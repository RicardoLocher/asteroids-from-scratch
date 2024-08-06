//
// Created by ricar on 06.08.2024.
//

#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

#include "iostream"
#include "cmath"

#include "SFML/Graphics.hpp"

class Player {
public:
    Player() : playerSprite() {
        if (!playerTexture.loadFromFile("../../assets/ship.png")) {
            std::cout << "Error loading Player Texture\n";
            return;
        }
        playerSprite.setTexture(playerTexture);
        playerSprite.setPosition(100, 100);
        pIsMovingForward = false;
        pIsMovingBackwards = false;
        pIsRotatingLeft = false;
        pIsRotatingRight = false;

        const sf::FloatRect bounds = playerSprite.getLocalBounds();
        playerSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    void handleInput(const sf::Keyboard::Key key, const bool isPressed) {
        if (key == sf::Keyboard::W) pIsMovingForward = isPressed;
        else if (key == sf::Keyboard::S) pIsMovingBackwards = isPressed;
        else if (key == sf::Keyboard::D) pIsRotatingRight = isPressed;
        else if (key == sf::Keyboard::A) pIsRotatingLeft = isPressed;
    }

    void update(const sf::Time deltaTime, const int windowWidth, const int windowHeight) {
        sf::Vector2f movement(0.f, 0.f);

        float rotation = playerSprite.getRotation();
        float radian = 3.14159265 * (rotation / 180);

        const sf::Vector2f forwardDirection(std::cos(radian), std::sin(radian));

        if (pIsMovingForward) movement += forwardDirection * movementSpeed;
        if (pIsMovingBackwards) movement -= forwardDirection * movementSpeed;

        sf::Vector2f newPosition = playerSprite.getPosition() + movement * deltaTime.asSeconds();
        checkWindowBounds(movement, newPosition, windowWidth, windowHeight);

        playerSprite.setPosition(newPosition);

        if (pIsRotatingLeft) playerSprite.rotate(-rotationSpeed * deltaTime.asSeconds());
        if (pIsRotatingRight) playerSprite.rotate(rotationSpeed * deltaTime.asSeconds());
    }

    void render(sf::RenderWindow& window) const {
        window.draw(playerSprite);
    }

private:
    void checkWindowBounds(sf::Vector2f& movement, sf::Vector2f& newPosition, const int windowWidth, const int windowHeight) const {
        if (newPosition.x < 0.f) {
            movement.x = 0.f;
            newPosition.x = 0.f;
        }
        if (newPosition.x + (playerRadius * 2) > static_cast<float>(windowWidth)) {
            movement.x = 0.f;
            newPosition.x = static_cast<float>(windowWidth) - (playerRadius * 2);
        }
        if (newPosition.y < 0.f) {
            movement.y = 0.f;
            newPosition.y = 0.f;
        }
        if (newPosition.y + (playerRadius * 2) > static_cast<float>(windowHeight)) {
            movement.y = 0.f;
            newPosition.y = static_cast<float>(windowHeight) - (playerRadius * 2);
        }
    }

    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    bool pIsMovingForward;
    bool pIsMovingBackwards;
    bool pIsRotatingRight;
    bool pIsRotatingLeft;

    float playerRadius = 10.f;
    float movementSpeed = 1000.f;
    float rotationSpeed = 200.f;
};

#endif //PLAYERCLASS_H
