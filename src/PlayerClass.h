//
// Created by RicardoLocher on 06.08.2024.
//

#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

#include "iostream"
#include "cmath"
#include "LaserClass.h"
#include "vector"

#include "SFML/Graphics.hpp"

class Player {
public:

    int playerLevel;
    int initialPlayerLevel = 1;
    bool hasActiveLaser = false;
    int points = 0;

    Player(const int WINDOW_WIDTH, const int WINDOW_HEIGHT) : playerSprite() {
        if (!playerTexture.loadFromFile("../../assets/ship.png")) {
            std::cout << "Error loading Player Texture\n";
            return;
        }

        // Handling the loading of the laser texture here instead of laser for performance
        if (!laserTexture.loadFromFile("../../assets/laser1.png")) {
            std::cout << "Error loading laser texture!" << std::endl;
            return;
        }
        laserTexture.setSmooth(true);

        playerSprite.setTexture(playerTexture);
        playerSprite.setPosition(static_cast<float>(WINDOW_WIDTH) / 2.f, static_cast<float>(WINDOW_HEIGHT) / 2.f);

        image = playerTexture.copyToImage(); // Extract Pixel data for collision detection.

        pIsMovingForward = false;
        pIsMovingBackwards = false;
        pIsRotatingLeft = false;
        pIsRotatingRight = false;
        pIsShooting = false;
        playerLevel = initialPlayerLevel;

        const sf::FloatRect bounds = playerSprite.getLocalBounds();
        playerSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    void handleInput(const sf::Keyboard::Key key, const bool isPressed) {
        if (key == sf::Keyboard::W) pIsMovingForward = isPressed;
        else if (key == sf::Keyboard::S) pIsMovingBackwards = isPressed;
        else if (key == sf::Keyboard::D) pIsRotatingRight = isPressed;
        else if (key == sf::Keyboard::A) pIsRotatingLeft = isPressed;
        else if (key == sf::Keyboard::Space && isPressed && !hasActiveLaser) shootLaser();
    }

    void update(const sf::Time deltaTime, const int windowWidth, const int windowHeight) {
        sf::Vector2f movement(0.f, 0.f);

        const float rotation = playerSprite.getRotation();
        const float radian = 3.14159265 * (rotation / 180); // NOLINT(*-narrowing-conversions)

        const sf::Vector2f forwardDirection(std::cos(radian), std::sin(radian));

        if (pIsMovingForward) movement += forwardDirection * movementSpeed;
        if (pIsMovingBackwards) movement -= forwardDirection * movementSpeed;

        sf::Vector2f newPosition = playerSprite.getPosition() + movement * deltaTime.asSeconds();
        checkWindowBounds(newPosition, windowWidth, windowHeight);

        playerSprite.setPosition(newPosition);

        if (pIsRotatingLeft) playerSprite.rotate(-rotationSpeed * deltaTime.asSeconds());
        if (pIsRotatingRight) playerSprite.rotate(rotationSpeed * deltaTime.asSeconds());

        updateLasers(deltaTime, windowWidth, windowHeight);
    }

    void render(sf::RenderWindow& window) const {
        for (const auto& laser : activeLasers) {
            laser.render(window);
        }
        window.draw(playerSprite);
    }

    void reset(int WINDOW_WIDTH, int WINDOW_HEIGHT) {
        playerSprite.setPosition(static_cast<float>(WINDOW_WIDTH) / 2.f, static_cast<float>(WINDOW_HEIGHT) / 2.f);
        playerLevel = initialPlayerLevel;
        points = 0;
    }

    sf::FloatRect getBounds() const {
        return playerSprite.getGlobalBounds();
    }

    const sf::Image& getImage() const {
        return image;
    }

    const sf::Sprite& getSprite() const {
        return playerSprite;
    }

    std::vector<Laser>& getLasers() {
        return activeLasers;
    }

private:
    static void checkWindowBounds(sf::Vector2f& position, const int windowWidth, const int windowHeight) {
        if (position.x < 0.f) {
            position.x = static_cast<float>(windowWidth);
        }
        if (position.x > static_cast<float>(windowWidth)) {
            position.x = 0.f;
        }
        if (position.y < 0.f) {
            position.y = static_cast<float>(windowHeight);
        }
        if (position.y > static_cast<float>(windowHeight)) {
            position.y = 0.f;
        }
    }

    void shootLaser() {
        // Only shoot a laser if there is no active one
        activeLasers.emplace_back(playerSprite.getPosition(), playerSprite.getRotation(), laserTexture);
        hasActiveLaser = true;
    }

    void updateLasers(const sf::Time deltaTime, const int WindowWidth, const int WindowHeight) {
        for (auto it = activeLasers.begin(); it != activeLasers.end(); ) {
            it->update(deltaTime);
            sf::Vector2f pos = it->getPosition();
            if (pos.x < 0.f || pos.x > WindowWidth || pos.y < 0.f || pos.y > WindowHeight) { // NOLINT(*-narrowing-conversions)
                it = activeLasers.erase(it);
                hasActiveLaser = false;
            } else {
                ++it;
            }
        }
    }

    sf::Texture playerTexture;
    sf::Texture laserTexture;
    sf::Sprite playerSprite;
    sf::Image image;
    std::vector<Laser> activeLasers;

    bool pIsMovingForward;
    bool pIsMovingBackwards;
    bool pIsRotatingRight;
    bool pIsRotatingLeft;
    bool pIsShooting;

    float playerRadius = 10.f;
    float movementSpeed = 500.f;
    float rotationSpeed = 200.f;
};

#endif //PLAYERCLASS_H
