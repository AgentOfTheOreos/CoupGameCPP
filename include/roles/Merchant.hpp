// alaasalah142@gmail.com
// include/Merchant.hpp
#pragma once

#include "../Role.hpp"
#include "../Player.hpp"

class Merchant : public Role {
public:
    Merchant() = default;
    Merchant(const Merchant&) = default;
    Merchant& operator=(const Merchant&) = default;
    ~Merchant() override = default;

    // Polymorphic clone
    Merchant* clone() const override {
        return new Merchant(*this);
    }

    /**
     * If this Merchant is attacked by an arrest, he pays two coins to the pot
     * instead of losing one to another player.
     */
    void blockArrest(Player& blocker, Player& actor) override {
        // Refund the stolen coin
        blocker.addCoins(1);
        actor.payCoins(1);            // thief must give it back

        // Pay 2 coins to the pot (modelled by self‑payment here)
        blocker.payCoins(2);
    }
};