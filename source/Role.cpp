// alaasalah142@gmail.com
// src/Role.cpp
#include "../include/Role.hpp"
#include "../include/Player.hpp"
#include <stdexcept>

// Default implementations for core actions
void Role::gather(Player& actor) {
    if (actor.sanctionActive()) {
        throw std::runtime_error("Sanction is active: player cannot perform economic actions in this turn");
    }
    // Generic gather: gain 1 coin
    actor.addCoins(1);
    actor.resetFlags();
}

void Role::tax(Player& actor) {
    if (actor.sanctionActive()) {
        throw std::runtime_error("Sanction is active: player cannot perform economic actions in this turn");
    }
    if (actor.taxBlocked()) {
        throw std::runtime_error("Tax action is blocked: player cannot perform tax action in this turn");
    }
    // Generic tax: gain 2 coins
    actor.addCoins(2);
    actor.resetFlags();
}

void Role::bribe(Player& actor) {
    if (actor.sanctionActive()) {
        throw std::runtime_error("Sanction is active: player cannot perform economic actions in this turn");
    }
    if (actor.bribeBlocked()) {
        throw std::runtime_error("Bribe action is blocked: player cannot perform bribe action in this turn");
    }
    // Generic bribe: pay 4 coins
    actor.payCoins(4);
    actor.extraAction(true);
    actor.resetFlags();
}

void Role::arrest(Player& actor, Player& target) {
    // Generic arrest: take 1 coin from target
    if (!target.hasCoins(1)) {
        throw std::runtime_error("Target does not have a coin to arrest");
    }
    if (actor.arrestBlocked()) {
        throw std::runtime_error("Arrest action is blocked: player cannot perform arrest action in this turn");
    }
    target.payCoins(1);
    actor.addCoins(1);
    actor.resetFlags();
}

void Role::sanction(Player& actor, Player& target) {
    // Generic sanction: pay 3 coins
    actor.payCoins(3);
    target.setSanctionFlag(true);
    // Effects on target handled via flags/hooks in Game/Player
    actor.resetFlags();
}

void Role::coup(Player& actor, Player& target) {
    // Generic coup: pay 7 coins and eliminate target
    actor.payCoins(7);
    target.eliminate();
    actor.resetFlags();
}

// Default block reactions: not supported, throw
void Role::blockGather(Player& blocker, Player& actor) {
    throw std::runtime_error("This role cannot block Gather");
}

void Role::blockTax(Player& blocker, Player& actor) {
    throw std::runtime_error("This role cannot block Tax");
}

void Role::blockBribe(Player& blocker, Player& actor) {
    throw std::runtime_error("This role cannot block Bribe");
}

void Role::blockArrest(Player& blocker, Player& actor) {
    throw std::runtime_error("This role cannot block Arrest");
}

void Role::blockSanction(Player& blocker, Player& actor) {
    throw std::runtime_error("This role cannot block Sanction");
}

void Role::blockCoup(Player& blocker, Player& actor) {
    throw std::runtime_error("This role cannot block Coup");
}
