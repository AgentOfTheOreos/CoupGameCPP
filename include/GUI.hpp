// alaasalah142@gmail.com
#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"

/// A minimal GUI for Coup: shows turn, coin counts, and six actions.
/// Actions are chosen by pressing keys 1–6.
class GUI {

private:
    static const int ACTION_COUNT = 8;

public:
    GUI(Game& game);

    /// Main loop: returns when window is closed.
    void run();

private:
    Game&              _game;
    sf::RenderWindow   _window;
    sf::Font           _font;
    sf::Text           _title;
    sf::Text           _turnInfo;
    sf::Text           _playerInfo;
    sf::Text           _actionTexts[ACTION_COUNT];

    // One text entry per player to show full status
    std::vector<sf::Text> _playerStatusTexts;

    void loadResources();
    void layoutText();
    void handleInput();
    void renderFrame();
};
