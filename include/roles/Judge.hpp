// alaasalah142@gmail.com
// include/Judge.hpp
#pragma once

#include "../Role.hpp"
#include "../Player.hpp"

class Judge : public Role {
public:
    Judge() = default;
    Judge(const Judge&) = default;
    Judge& operator=(const Judge&) = default;
    ~Judge() override = default;

    // Polymorphic clone
    Judge* clone() const override {
        return new Judge(*this);
    }

    /**
     * Cancel another player's bribe: prevents its effect (no further coin changes required).
     * The actor has already paid the 4 coins when they invoked bribe().
     */
    void blockBribe(Player& blocker, Player& actor) override {
        actor.extraAction(false);
        actor.setBribeBlockedFlag(true);
    }

    /**
     * If this Judge is targeted by a sanction, the sanctioner pays an extra coin.
     */
    void blockSanction(Player& blocker, Player& actor) override {
        // 'actor' is the player who imposed the sanction
        actor.payCoins(1);
    }
};