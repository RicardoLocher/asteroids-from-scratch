//
// Created by RicardoLocher on 07.08.2024.
//

#ifndef ASTEROIDCLASS_H
#define ASTEROIDCLASS_H
#include "SFML/Graphics.hpp"

class Asteroid {
public:
    Asteroid(const sf::Vector2f pos, const sf::Texture& texture, const float dir) {
        sprite.setTexture(texture);
        sprite.setPosition(pos);

        const float radian = 3.14159265 * (dir / 180); // NOLINT(*-narrowing-conversions)
        const sf::Vector2f forward(std::cos(radian), std::sin(radian));
        direction = forward;

        const sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    void update(const sf::Time deltaTime, const int WINDOW_WIDTH, const int WINDOW_HEIGHT) {
        sprite.rotate(0.1);
        sprite.move(direction * speed * deltaTime.asSeconds());

        sf::Vector2f newPosition = sprite.getPosition();
        checkWindowBounds(newPosition, WINDOW_WIDTH, WINDOW_HEIGHT);

        sprite.setPosition(newPosition);
    }

    void render(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
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

    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed = 200.f;

};

#endif //ASTEROIDCLASS_H
