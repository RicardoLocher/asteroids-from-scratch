//
// Created by RicardoLocher on 23.10.2024.
//
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Star.h"

#ifndef STARBACKGROUND_H
#define STARBACKGROUND_H

class StarBackground {
private:
    std::vector<Star> stars;
    sf::RenderWindow& window;
    unsigned int starCount;

public:
    StarBackground(sf::RenderWindow& win, unsigned int count) : window(win), starCount(count) {
        std::srand(static_cast<unsigned int>(std::time(0))); // Seed for random generation
        generateStars();
    }

    // Generate stars based on the window size
    void generateStars() {
        stars.clear();
        sf::Vector2u size = window.getSize();
        for (unsigned int i = 0; i < starCount; ++i) {
            float x = static_cast<float>(std::rand() % size.x);
            float y = static_cast<float>(std::rand() % size.y);
            stars.emplace_back(x, y);
        }
    }

    // Draw the stars
    void draw(sf::RenderWindow& window) {
        for (const auto& star : stars) {
            sf::RectangleShape pixel(sf::Vector2f(2, 2)); // Star size is 2x2 pixels
            pixel.setPosition(star.position);
            pixel.setFillColor(sf::Color::White);
            window.draw(pixel);
        }
    }

    // Update stars when the window is resized
    void updateOnResize() {
        generateStars();
    }
};

#endif //STARBACKGROUND_H
