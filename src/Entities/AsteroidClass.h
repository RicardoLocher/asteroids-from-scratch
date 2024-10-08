//
// Created by RicardoLocher on 07.08.2024.
//

#ifndef ASTEROIDCLASS_H
#define ASTEROIDCLASS_H
#include "SFML/Graphics.hpp"

class Asteroid {
public:
    bool isMini;
    Asteroid(sf::Vector2f pos, const sf::Texture& texture, float dir, bool mini);

    void update(sf::Time deltaTime, int WINDOW_WIDTH, int WINDOW_HEIGHT);

    void render(sf::RenderWindow& window) const;

    // Getters
    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
    const sf::Image& getImage() const {
        return image;
    }
    const sf::Sprite& getSprite() const {
        return sprite;
    }

private:

    static void checkWindowBounds(sf::Vector2f& position, int windowWidth, int windowHeight);

    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed = 300.f;

    sf::Image image;
};

#endif //ASTEROIDCLASS_H
