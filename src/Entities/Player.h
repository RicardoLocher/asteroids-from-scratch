//
// Created by RicardoLocher on 06.08.2024.
//

#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

#include "Laser.h"
#include "vector"

#include "../../cmake-build-debug/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class Player {
public:

    int playerLevel;
    int initialPlayerLevel = 1;
    bool hasActiveLaser = false;
    int points = 0;

    Player(int WINDOW_WIDTH, int WINDOW_HEIGHT);

    void handleInput(sf::Keyboard::Key key, bool isPressed);

    void update(sf::Time deltaTime, int windowWidth, int windowHeight);

    void render(sf::RenderWindow& window) const;

    void reset(int WINDOW_WIDTH, int WINDOW_HEIGHT);

    // Getters
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
    static void checkWindowBounds(sf::Vector2f& position, int windowWidth, int windowHeight);

    void shootLaser();
    void updateLasers(sf::Time deltaTime, int WindowWidth, int WindowHeight);

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

    sf::SoundBuffer shotSoundBuffer;
    sf::Sound shotSound;

    float playerRadius = 10.f;
    float movementSpeed = 500.f;
    float rotationSpeed = 200.f;
};

#endif //PLAYERCLASS_H
