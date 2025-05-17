// alaasalah142@gmail.com
// include/Baron.hpp
#pragma once

#include "../Player.hpp"
#include "../Role.hpp"

class Baron : public Role {
public:
    Baron() = default;
    Baron(const Baron&) = default;
    Baron& operator=(const Baron&) = default;
    ~Baron() override = default;

    // Polymorphic clone
    Baron* clone() const override {
        return new Baron(*this);
    }

    /**
     * Role-specific action:
     * "Invest" 3 coins to receive 6 coins in return.
     */
    void invest(Player& actor) override {
        actor.payCoins(3);
        actor.addCoins(6);
    }

    /**
     * Override blockSanction:
     * If this Baron is targeted by a sanction, he gains 1 coin as compensation.
     */
    void blockSanction(Player& blocker, Player& actor) override {
        blocker.addCoins(1);
    }
};