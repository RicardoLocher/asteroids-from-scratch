//
// Created by RicardoLocher on 23.10.2024.
//

#ifndef STAR_H
#define STAR_H
#include "SFML/System/Vector2.hpp"

class Star {
public:
    sf::Vector2f position;
    Star(float x, float y) : position(x, y) {}
};

#endif //STAR_H
