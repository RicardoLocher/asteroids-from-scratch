//
// Created by RicardoLocher on 08.10.2024.
//

#include <iostream>

#include "Player.h"

Player::Player(int WINDOW_WIDTH, int WINDOW_HEIGHT) : playerSprite() {
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

// Getters are declared and initialized in the headerfile.

// Game cycle methods
void Player::handleInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::W) pIsMovingForward = isPressed;
    else if (key == sf::Keyboard::S) pIsMovingBackwards = isPressed;
    else if (key == sf::Keyboard::D) pIsRotatingRight = isPressed;
    else if (key == sf::Keyboard::A) pIsRotatingLeft = isPressed;
    else if (key == sf::Keyboard::Space && isPressed && !hasActiveLaser) shootLaser();
}
void Player::update(sf::Time deltaTime, int windowWidth, int windowHeight) {
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
void Player::render(sf::RenderWindow& window) const {
    for (const auto& laser : activeLasers) {
        laser.render(window);
    }
    window.draw(playerSprite);
}

// Game logic methods
void Player::reset(int WINDOW_WIDTH, int WINDOW_HEIGHT) {
    playerSprite.setPosition(static_cast<float>(WINDOW_WIDTH) / 2.f, static_cast<float>(WINDOW_HEIGHT) / 2.f);
    playerLevel = initialPlayerLevel;
    points = 0;
}
void Player::checkWindowBounds(sf::Vector2f& position, int windowWidth, int windowHeight) {
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

// Laser handling
void Player::shootLaser() {
    // Only shoot a laser if there is no active one
    activeLasers.emplace_back(playerSprite.getPosition(), playerSprite.getRotation(), laserTexture);
    hasActiveLaser = true;
}
void Player::updateLasers(sf::Time deltaTime, int WindowWidth, int WindowHeight) {
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