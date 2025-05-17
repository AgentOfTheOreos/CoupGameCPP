// alaasalah142@gmail.com
// include/Role.hpp
#pragma once

#include <stdexcept>

class Player;  // forward declaration

class Role {
public:
    virtual ~Role() = default;

    // Polymorphic clone for Rule of Three
    virtual Role* clone() const = 0;

    // Core actions
    virtual void gather(Player& actor);
    virtual void tax(Player& actor);
    virtual void bribe(Player& actor);
    virtual void arrest(Player& actor, Player& target);
    virtual void sanction(Player& actor, Player& target);
    virtual void coup(Player& actor, Player& target);

    // Block reactions
    virtual void blockGather(Player& blocker, Player& actor);
    virtual void blockTax(Player& blocker, Player& actor);
    virtual void blockBribe(Player& blocker, Player& actor);
    virtual void blockArrest(Player& blocker, Player& actor);
    virtual void blockSanction(Player& blocker, Player& actor);
    virtual void blockCoup(Player& blocker, Player& actor);

    virtual void invest(Player& actor) {
        throw std::runtime_error("This role cannot invest");
    }

    virtual void spyCoins(Player& actor, Player& target) {
        throw std::runtime_error("This role cannot spy");
    }
};