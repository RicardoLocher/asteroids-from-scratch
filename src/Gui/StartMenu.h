//
// Created by RicardoLocher on 15.11.2024.
//

#ifndef STARTMENU_H
#define STARTMENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Starbackground.h"

class StartMenu {
public:
    StartMenu(sf::RenderWindow& window);

    // Display the menu and return the selected option
    int displayMenu();

private:
    sf::RenderWindow& m_window;
    sf::Font m_font;
    std::vector<sf::Text> m_menuItems;
    int m_selectedIndex;

    StarBackground background;

    sf::Texture titleTexture;
    sf::Sprite titleSprite;

    // Helper methods
    void handleInput();
    void renderMenu();
};

#endif // STARTMENU_H