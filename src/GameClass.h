//
// Created by RicardoLocher on 05.08.2024.
//

#ifndef GAMECLASS_H
#define GAMECLASS_H

#include "SFML/Graphics.hpp"

#include "Entities/Player.h"
#include "Entities/AsteroidClass.h"
#include "Entities/Laser.h"
#include "Gui/Starbackground.h"

class Game {
public:
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;
    sf::RenderWindow window;

    Game();

    void run();

    // Asteroids logic (will soon be refactored and moved to another class)
    void generateAsteroids(int level);
    void generateMiniAsteroids(sf::Vector2f pos);
    static sf::Vector2f generateAsteroid_Pos(const sf::Vector2f& playerPos, float minDistance);
    static float generateAsteroid_Direction();

    // Game flow control
    void resetGame();
    void nextLevel();
    void startMenu(sf::RenderWindow& window);

private:
    // game cycle logic
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    // ReSharper disable CppDFAConstantParameter
    void updateAsteroids(sf::Time deltaTime, int WindowWidth, int WindowHeight);

    // Collision logic (will soon be moved from this class)
    // ReSharper disable once CppMemberFunctionMayBeConst
    void checkCollisions();
    static bool pixelPerfectCollision(const sf::Sprite& sprite1, const sf::Image& image1, const sf::Sprite& sprite2, const sf::Image& image2);


    sf::Time TimePerFrame = sf::seconds(1.f / 144.f);

    Player player;

    sf::Font font;
    sf::Text fpsText;
    sf::Clock fpsClock;
    int frameCount;
    float fps;

    StarBackground starBackground;

    std::vector<Asteroid> asteroids;
    sf::Texture asteroid1;
    sf::Texture asteroid2;

    bool gameOver;
    sf::Text gameOverText;

    sf::Text pointsText;
    sf::Text levelText;
};

#endif //GAMECLASS_H
