// alaasalah142@gmail.com
// tests/test_player.cpp
// Compile with:  clang++ -std=c++23 -I../include -o test_player test_player.cpp
//                ./test_player
//
// This file exercises only the Player class.  We stub a DummyRole that does
// nothing except satisfy the interface and clone itself.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/misc/DummyRole.hpp"
#include "../include/Player.hpp"
#include "../include/Role.hpp"

// ------------------------------------------------------------------
// Test cases
// ------------------------------------------------------------------
TEST_CASE("Constructor & basic accessors") {
    Player p("Alice", new DummyRole{});

    CHECK(p.name()      == "Alice");
    CHECK(p.coins()     == 0);
    CHECK(p.isActive()  == true);
    CHECK(p.arrestBlocked() == false);
    CHECK(p.taxBlocked()    == false);
    CHECK(p.bribeBlocked()  == false);
    CHECK(p.sanctionActive()== false);
}

TEST_CASE("Coin bookkeeping: add / pay / has") {
    Player p("Bob", new DummyRole{});

    p.addCoins(5);
    CHECK(p.coins() == 5);
    CHECK(p.hasCoins(4) == true);
    CHECK(p.hasCoins(7) == false);

    p.payCoins(3);
    CHECK(p.coins() == 2);

    CHECK_THROWS_AS(p.payCoins(5), std::runtime_error);
    CHECK_THROWS_AS(p.addCoins(-1), std::invalid_argument);
}

TEST_CASE("Rule of Three: deep copy semantics") {
    Player original("Carol", new DummyRole{});
    original.addCoins(4);
    original.setBribeBlockedFlag(true);

    SUBCASE("Copy constructor") {
        Player copy(original);

        CHECK(copy.name()    == "Carol");
        CHECK(copy.coins()   == 4);
        CHECK(copy.bribeBlocked() == true);

        // Mutate one and ensure independence
        copy.addCoins(1);
        original.payCoins(2);
        CHECK(copy.coins()      == 5);
        CHECK(original.coins()  == 2);
    }

    SUBCASE("Copy assignment") {
        Player other("Dave", new DummyRole{});
        other = original;

        CHECK(other.name()        == "Carol");
        CHECK(other.coins()       == 4);
        CHECK(other.bribeBlocked()== true);

        other.addCoins(2);
        original.payCoins(1);
        CHECK(other.coins()    == 6);
        CHECK(original.coins() == 3);
    }
}

TEST_CASE("Eliminate and revive") {
    Player p("Eve", new DummyRole{});
    CHECK(p.isActive() == true);

    p.eliminate();
    CHECK(p.isActive() == false);

    p.revive();
    CHECK(p.isActive() == true);
}

TEST_CASE("State‑flag setters & resetFlags()") {
    Player p("Frank", new DummyRole{});

    p.setArrestBlockedFlag(true);
    p.setTaxBlockedFlag(true);
    p.setBribeBlockedFlag(true);
    p.setSanctionFlag(true);

    CHECK(p.arrestBlocked()  == true);
    CHECK(p.taxBlocked()     == true);
    CHECK(p.bribeBlocked()   == true);
    CHECK(p.sanctionActive() == true);

    p.resetFlags();

    CHECK(p.arrestBlocked()  == false);
    CHECK(p.taxBlocked()     == false);
    CHECK(p.bribeBlocked()   == false);
    CHECK(p.sanctionActive() == false);
}