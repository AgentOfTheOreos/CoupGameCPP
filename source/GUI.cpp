// alaasalah142@gmail.com
#include <iostream>
#include <array>
#include <vector>
#include <sstream>
#include "../include/GUI.hpp"
#include "../include/roles/Governor.hpp"
#include "../include/roles/Spy.hpp"
#include "../include/roles/Baron.hpp"
#include "../include/roles/General.hpp"
#include "../include/roles/Judge.hpp"
#include "../include/roles/Merchant.hpp"

GUI::GUI(Game& game)
: _game(game)
, _window({800, 600}, "Coup Demo")
{
    // Prepare one status text entry per player
    _playerStatusTexts.reserve(_game.playerCount());
    for (size_t i = 0; i < _game.playerCount(); ++i) {
        sf::Text t;
        t.setFont(_font);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::Black);
        _playerStatusTexts.push_back(t);
    }
    loadResources();
    layoutText();
}

void GUI::loadResources() {
    if (! _font.loadFromFile("assets/arial.ttf")) {
        throw std::runtime_error("Could not load font assets/arial.ttf");
    }

    // Title
    _title.setFont(_font);
    _title.setCharacterSize(32);
    _title.setFillColor(sf::Color::Black);
    _title.setString("Coup Demo");
    _title.setPosition(250, 20);

    // Turn info
    _turnInfo.setFont(_font);
    _turnInfo.setCharacterSize(28);
    _turnInfo.setFillColor(sf::Color::Black); 

    // Player info
    _playerInfo.setFont(_font);
    _playerInfo.setCharacterSize(20);
    _playerInfo.setFillColor(sf::Color::Black);

    // Actions
    for (int i = 0; i < 6; ++i) {
        _actionTexts[i].setFont(_font);
        _actionTexts[i].setCharacterSize(24);
        _actionTexts[i].setFillColor(sf::Color::Black);
        _actionTexts[i].setPosition(100, 150 + i * 40);
    }
}

void GUI::layoutText() {
    // Turn info
    _turnInfo.setFont(_font);
    _turnInfo.setCharacterSize(28);
    _turnInfo.setPosition(50, 80);

    // Player coin info
    _playerInfo.setFont(_font);
    _playerInfo.setCharacterSize(20);
    _playerInfo.setPosition(50, 400);

    // Action labels
    static const std::array<std::string,ACTION_COUNT> labels = {
        "1) Gather (+1 coin)",
        "2) Tax    (+2 coins)",
        "3) Bribe  (pay 4 coins)",
        "4) Arrest target",
        "5) Sanction target",
        "6) Coup   target",
        "7) Invest (Baron only)",
        "8) Spy target (Spy only)"
    };
    for (int i = 0; i < ACTION_COUNT; ++i) {
        _actionTexts[i].setFont(_font);
        _actionTexts[i].setCharacterSize(24);
        _actionTexts[i].setFillColor(sf::Color::Black);
        _actionTexts[i].setPosition(100, 150 + i * 40);
        _actionTexts[i].setString(labels[i]);
    }
}

void GUI::run() {
    while (_window.isOpen()) {
        handleInput();
        renderFrame();
    }
}

void GUI::handleInput() {
    sf::Event ev;
    while (_window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) {
            _window.close();
            return;
        }
        if (ev.type == sf::Event::KeyPressed) {
            try {
                switch (ev.key.code) {
                    case sf::Keyboard::Num1: _game.gather();   break;
                    case sf::Keyboard::Num2: _game.tax();      break;
                    case sf::Keyboard::Num3: _game.bribe();    break;
                    case sf::Keyboard::Num4: {
                        // choose a target by console prompt
                        size_t idx; std::cout << "Arrest whom? "; std::cin >> idx;
                        _game.arrest(idx);
                        } break;
                    case sf::Keyboard::Num5: {
                        size_t idx; std::cout << "Sanction whom? "; std::cin >> idx;
                        _game.sanction(idx);
                        } break;
                    case sf::Keyboard::Num6: {
                        size_t idx; std::cout << "Coup whom? "; std::cin >> idx;
                        _game.coup(idx);
                        } break;
                    case sf::Keyboard::Num7:
                        _game.invest();
                        break;
                    case sf::Keyboard::Num8: {
                        size_t idx;
                        std::cout << "Spy on whom? ";
                        std::cin >> idx;
                        _game.spy(idx);
                        break;
                    }
                    default: break;
                }
            } catch (const std::exception& e) {
                // show error on console; GUI could display a pop-up in a later iteration
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
}

void GUI::renderFrame() {
    // Determine active player index
    size_t activeIdx = _game.turnIdx();

    const Player& me = _game.getPlayer(activeIdx);
    bool isBaron = dynamic_cast<const Baron*>(me.role());
    bool isSpy   = dynamic_cast<const Spy*>(me.role());

    // Grey-out role-specific options as before…
    _actionTexts[6].setFillColor(isBaron ? sf::Color::Black : sf::Color(150,150,150));
    _actionTexts[7].setFillColor(isSpy   ? sf::Color::Black : sf::Color(150,150,150));

    _window.clear(sf::Color::White);

    // Check for a winner
    try {
        auto name = _game.winner();   // throws if >1 active
        // We have a winner! Draw only the victory screen.
        sf::Text win;
        win.setFont(_font);
        win.setCharacterSize(48);
        win.setFillColor(sf::Color::Red);
        win.setString("Winner: " + name);
        // center it roughly:
        auto bounds = win.getLocalBounds();
        win.setPosition(
            (_window.getSize().x - bounds.width) / 2,
            (_window.getSize().y - bounds.height) / 2
        );
        _window.draw(win);
        _window.display();
        return;  // skip drawing the rest of the GUI
    } catch (const std::exception&) {
        // no winner yet — fall through to normal rendering
    }
    
    // Update turn info
    _turnInfo.setString("Turn: " + _game.turn());

    // Draw title, turn, actions, etc.
    _window.draw(_title);
    _window.draw(_turnInfo);
    for (int i = 0; i < ACTION_COUNT; ++i)
        _window.draw(_actionTexts[i]);

    // Draw detailed status for each player
    for (size_t i = 0; i < _game.playerCount(); ++i) {
        const Player& p = _game.getPlayer(i);

        // RTTI
        const Role* r = p.role();
        std::string roleName;

        // determine the concrete role via RTTI
        if      (dynamic_cast<const Governor*>(r)) roleName = "Governor";
        else if (dynamic_cast<const Spy*>(r))      roleName = "Spy";
        else if (dynamic_cast<const Baron*>(r))    roleName = "Baron";
        else if (dynamic_cast<const General*>(r))  roleName = "General";
        else if (dynamic_cast<const Judge*>(r))    roleName = "Judge";
        else if (dynamic_cast<const Merchant*>(r)) roleName = "Merchant";
        else                                       roleName = "Unknown";

        std::ostringstream ss;
        ss << i << ": " << p.name()
           << " (" << roleName << ")  ";

        if (i == activeIdx) {
            ss << p.coins() << " coins";
        } else {
            ss << "?? coins";
        }

        // flags/elimination as before
        if (!p.isActive())      ss << " [ELIM]";
        if (p.arrestBlocked())  ss << " [NoArrest]";
        if (p.taxBlocked())     ss << " [NoTax]";
        if (p.bribeBlocked())   ss << " [NoBribe]";
        if (p.sanctionActive()) ss << " [Sanction]";

        _playerStatusTexts[i].setString(ss.str());
        _playerStatusTexts[i].setPosition(400, 100 + i*30);
        _window.draw(_playerStatusTexts[i]);
    }

    _window.display();
}
