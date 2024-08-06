//
// Created by RicardoLocher on 06.08.2024.
//

#ifndef LASERCLASS_H
#define LASERCLASS_H

#include "cmath"
#include "SFML/Graphics.hpp"

class Laser {
public:
    Laser(const sf::Vector2f pos, const float rotation, const sf::Texture& texture) {


        laserSprite.setTexture(texture);
        laserSprite.setPosition(pos);
        laserSprite.setRotation(rotation);

        const sf::FloatRect bounds = laserSprite.getLocalBounds();
        laserSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

        const float radian = rotation / 180 * 3.14159265; // NOLINT(*-narrowing-conversions)
        direction = sf::Vector2f(std::cos(radian), std::sin(radian));
    }

    void update(const sf::Time deltaTime) {
        laserSprite.move(direction  * speed * deltaTime.asSeconds());
    }

    void render(sf::RenderWindow& window) const {
        window.draw(laserSprite);
    }

    // Getter for the player class to check collisions with window borders
    sf::Vector2f getPosition() const {
        return laserSprite.getPosition();
    }

private:
    sf::Sprite laserSprite;
    sf::Vector2f direction;
    float speed = 1500.f;
};

#endif //LASERCLASS_H
