// alaasalah142@gmail.com
// include/Governor.hpp
#pragma once

#include "../Role.hpp"
#include "../Player.hpp"

class Governor : public Role {
public:
    Governor() = default;
    Governor(const Governor&) = default;
    Governor& operator=(const Governor&) = default;
    ~Governor() override = default;

    // Clone for polymorphic copying
    Governor* clone() const override {
        return new Governor(*this);
    }

    // Override tax: gain 3 coins instead of 2
    void tax(Player& actor) override {
        actor.addCoins(3);
    }

    // Override blockTax: cancel another player's tax action
    void blockTax(Player& blocker, Player& actor) override {
        actor.setTaxBlockedFlag(true);
    }
};