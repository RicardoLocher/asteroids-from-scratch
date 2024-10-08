//
// Created by RicardoLocher on 06.08.2024.
//

#ifndef LASERCLASS_H
#define LASERCLASS_H

#include "cmath"
#include "../../cmake-build-debug/_deps/sfml-src/include/SFML/Graphics.hpp"

class Laser {
public:
    Laser(sf::Vector2f pos, float rotation, const sf::Texture& texture);

    void update(sf::Time deltaTime);

    void render(sf::RenderWindow& window) const;

    // Getters
    sf::Vector2f getPosition() const {
        return laserSprite.getPosition();
    }
    const sf::Image& getImage() const {
        return image;
    }
    const sf::Sprite& getSprite() const {
        return laserSprite;
    }

private:
    sf::Sprite laserSprite;
    sf::Vector2f direction;
    float speed = 1500.f;
    sf::Image image;
};

#endif //LASERCLASS_H
