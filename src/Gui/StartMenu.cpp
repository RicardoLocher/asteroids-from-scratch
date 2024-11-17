//
// Created by RicardoLocher on 15.11.2024.
//

#include "StartMenu.h"

StartMenu::StartMenu(sf::RenderWindow& window) : m_window(window), m_selectedIndex(0), background(m_window, 100) {

    if (!backgroundMusic.openFromFile("../../assets/OutThere.ogg")) {
        throw std::runtime_error("Failed to load background music");
    }

    if (!m_font.loadFromFile("../../assets/manolomono.otf")) {
        throw std::runtime_error("Failed to load font");
    }

    if (!titleTexture.loadFromFile("../../assets/Title.png")) {
        throw std::runtime_error("Failed to load title Texture");
    }
    titleSprite.setTexture(titleTexture);
    titleSprite.setPosition(
        m_window.getSize().x / 2.0f - titleSprite.getLocalBounds().width / 2.0f, // NOLINT(*-narrowing-conversions)
        m_window.getSize().y / 5.0f // NOLINT(*-narrowing-conversions)
    );

    // Create menu items
    std::vector<std::string> items = { "Start Game", "Exit" };
    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text;
        text.setFont(m_font);
        text.setString(items[i]);
        text.setCharacterSize(36);
        text.setFillColor(i == m_selectedIndex ? sf::Color::Yellow : sf::Color::White);
        text.setPosition(
            m_window.getSize().x / 2.0f - text.getGlobalBounds().width / 2.0f, // NOLINT(*-narrowing-conversions)
            m_window.getSize().y / 2.0f + i * 50.0f // NOLINT(*-narrowing-conversions)
        );
        m_menuItems.push_back(text);
    }
}

int StartMenu::displayMenu() {
    backgroundMusic.play();

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            // Handle window close event
            if (event.type == sf::Event::Closed) {
                m_window.close();
                return -1; // Exit the menu if the window is closed
            }

            // Handle key press events
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    m_selectedIndex = (m_selectedIndex - 1 + m_menuItems.size()) % m_menuItems.size();
                } else if (event.key.code == sf::Keyboard::Down) {
                    m_selectedIndex = (m_selectedIndex + 1) % m_menuItems.size();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    return m_selectedIndex;
                }
            }
        }

        // Update menu item colors
        for (size_t i = 0; i < m_menuItems.size(); ++i) {
            m_menuItems[i].setFillColor(i == m_selectedIndex ? sf::Color::Yellow : sf::Color::White);
        }

        // Render the menu
        m_window.clear();
        background.draw(m_window);
        m_window.draw(titleSprite);
        renderMenu();
        m_window.display();
    }

    return -1; // Return -1 if the window is closed
}

void StartMenu::renderMenu() {
    for (const auto& item : m_menuItems) {
        m_window.draw(item);
    }
}
