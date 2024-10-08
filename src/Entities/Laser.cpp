//
// Created by RicardoLocher on 08.10.2024.
//

#include "Laser.h"

Laser::Laser(sf::Vector2f pos, float rotation, const sf::Texture& texture) {
    laserSprite.setTexture(texture);
    laserSprite.setPosition(pos);
    laserSprite.setRotation(rotation);
    image = texture.copyToImage();

    const sf::FloatRect bounds = laserSprite.getLocalBounds();
    laserSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    const float radian = rotation / 180 * 3.14159265; // NOLINT(*-narrowing-conversions)
    direction = sf::Vector2f(std::cos(radian), std::sin(radian));
}

// Getters are declared and initialized in the header file

// Game cycle logic
void Laser::update(sf::Time deltaTime) {
    laserSprite.move(direction  * speed * deltaTime.asSeconds());
}
void Laser::render(sf::RenderWindow& window) const {
    window.draw(laserSprite);
}