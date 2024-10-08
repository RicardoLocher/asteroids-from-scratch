//
// Created by RicardoLocher on 08.10.2024.
//

#include "AsteroidClass.h"

#include <cmath>

Asteroid::Asteroid(sf::Vector2f pos, const sf::Texture& texture, float dir, bool mini) {
    sprite.setTexture(texture);
    sprite.setPosition(pos);

    isMini = mini;

    image = texture.copyToImage(); // Extract Pixel data for collision detection.

    const float radian = 3.14159265 * (dir / 180); // NOLINT(*-narrowing-conversions)
    const sf::Vector2f forward(std::cos(radian), std::sin(radian));
    direction = forward;

    const sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Asteroid::update(sf::Time deltaTime, int WINDOW_WIDTH, int WINDOW_HEIGHT) {
    sprite.rotate(0.1);
    sprite.move(direction * speed * deltaTime.asSeconds());

    sf::Vector2f newPosition = sprite.getPosition();
    checkWindowBounds(newPosition, WINDOW_WIDTH, WINDOW_HEIGHT);

    sprite.setPosition(newPosition);
}
void Asteroid::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Asteroid::checkWindowBounds(sf::Vector2f& position, int windowWidth, int windowHeight) {
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
