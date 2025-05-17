// alaasalah142@gmail.com
// include/General.hpp
#pragma once

#include "../Player.hpp"
#include "../Role.hpp"

class General : public Role {
public:
    General() = default;
    General(const General&) = default;
    General& operator=(const General&) = default;
    ~General() override = default;

    // Clone for polymorphic copying
    General* clone() const override {
        return new General(*this);
    }

    // Override blockCoup: pay 5 coins to prevent a coup
    void blockCoup(Player& blocker, Player& actor) override {
        // blocker is the General who blocks the coup
        blocker.payCoins(5);
        // Note: reversal of the coup effect (restoring the target and refunding actor)
        // will be handled at the Game level by invoking this block before finalizing
    }

    // Override blockArrest: if General is arrested, refund the 1 coin taken
    // logic will be finalized at the Game level
    void blockArrest(Player& blocker, Player& actor) override {
        // blocker is the General who is attacked
        blocker.addCoins(1);
    }
};