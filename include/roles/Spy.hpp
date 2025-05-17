// alaasalah142@gmail.com
// include/Spy.hpp
#pragma once

#include "../Player.hpp"
#include "../Role.hpp"
#include <iostream>

class Spy : public Role {
public:
    Spy() = default;
    Spy(const Spy&) = default;
    Spy& operator=(const Spy&) = default;
    ~Spy() override = default;

    // Clone for polymorphic copying
    Spy* clone() const override {
        return new Spy(*this);
    }

    /**
     * Reveal the target's coin count and block them from using arrest on their next turn.
     * This action does not consume coins or a turn.
     */
    void spyCoins(Player& actor, Player& target) override {
        // Reveal coin count to actor
        std::cout << actor.name() << " spies " << target.name()
                  << " has " << target.coins() << " coins." << std::endl;
        // Set a flag on target to prevent arrest usage next turn
        target.setArrestBlockedFlag(true);
    }

    // Override blockArrest: prevent another player from arresting this actor
    void blockArrest(Player& blocker, Player& actor) override {
        // By default, a blocked arrest refunds the 1 coin if any was taken
        blocker.addCoins(1);
    }
};