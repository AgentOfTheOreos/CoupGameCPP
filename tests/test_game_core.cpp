// alaasalah142@gmail.com
// tests/test_game_core.cpp
//
// Core integration tests for the Game class: turn flow, mandatory‑coup
// enforcement, coup‑blocking by a General, and winner detection.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/Game.hpp"
#include "../include/Role.hpp"
#include "../include/roles/General.hpp"
#include "../include/misc/DummyRole.hpp"

// --------------------------------------------------------------
// Test 1: Turn order advances correctly
// --------------------------------------------------------------
TEST_CASE("Turn order and basic actions") {
    Player p1 = makePlayer("P1", new DummyRole{});
    Player p2 = makePlayer("P2", new DummyRole{});
    Player p3 = makePlayer("P3", new DummyRole{});

    Game g{p1, p2, p3};

    CHECK(g.turn() == "P1");
    g.gather();                      // P1 acts
    CHECK(g.turn() == "P2");
    g.tax();                         // P2 acts
    CHECK(g.turn() == "P3");
    g.gather();                      // P3 acts
    CHECK(g.turn() == "P1");         // wraps back
}

// --------------------------------------------------------------
// Test 2: Mandatory coup when holding ≥10 coins
// --------------------------------------------------------------
TEST_CASE("Mandatory coup rule enforcement") {
    Player rich   = makePlayer("Rich",   new DummyRole{}, 10);
    Player victim = makePlayer("Victim", new DummyRole{});

    Game g{rich, victim};

    // Non‑coup economic action must throw
    CHECK_THROWS(g.gather());

    // A proper coup succeeds and ends the game
    CHECK_NOTHROW(g.coup(1));
    CHECK(g.players().size() == 1);
    CHECK(g.winner() == "Rich");
}

// --------------------------------------------------------------
// Test 3: General blocks a coup and revives the target
// --------------------------------------------------------------
TEST_CASE("General blocks a coup and revives target") {
    // Index order: 0 = attacker, 1 = target, 2 = general
    Player attacker = makePlayer("Att", new DummyRole{}, 10);
    Player target   = makePlayer("Tar", new DummyRole{});
    Player general  = makePlayer("Gen", new General{},  6);

    Game g{attacker, target, general};

    // Attacker coups target
    g.coup(1);
    CHECK(g.players().size() == 2);          // target eliminated

    // General blocks the coup
    g.generalBlocksCoup(2, 0, 1);
    auto names = g.players();
    CHECK(names.size() == 3);                // target revived

    // Game is still ongoing -> winner() should throw
    CHECK_THROWS(g.winner());
}