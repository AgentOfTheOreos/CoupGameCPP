// alaasalah142@gmail.com
// tests/test_roles.cpp
//
// Unit tests that focus on the *delta* behaviour of each concrete Role.
// These do **not** require the full Game engine – we drive the Role
// methods directly on Player objects.
//
// Build (after engine objects are compiled):
//   clang++ -std=c++23 -Iinclude -Wall -Wextra \
//            tests/test_roles.cpp src/*.o -o test_roles
//
// ---------------------------------------------------------------------

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/Role.hpp"
#include "../include/roles/Governor.hpp"
#include "../include/roles/Spy.hpp"
#include "../include/roles/Baron.hpp"
#include "../include/roles/General.hpp"
#include "../include/roles/Judge.hpp"
#include "../include/roles/Merchant.hpp"
#include "../include/misc/DummyRole.hpp"

// --------------------------------------------------------------
// Governor
// --------------------------------------------------------------
TEST_CASE("Governor: tax +3 and delayed tax‑block flag") {
    Player gov   = makePlayer("Gov",   new Governor{});
    Player other = makePlayer("Bob",   new DummyRole{});

    other.tax();                                      // +2 coins
    static_cast<Governor*>(gov.role())->blockTax(gov, other);

    CHECK(other.taxBlocked() == true);

    gov.tax();                                        // +3 coins
    CHECK(gov.coins() == 3);
}

// --------------------------------------------------------------
// Spy
// --------------------------------------------------------------
TEST_CASE("Spy: spyCoins reveals & blocks target arrest") {
    Player spy    = makePlayer("S", new Spy{});
    Player target = makePlayer("T", new DummyRole{}, 5);

    static_cast<Spy*>(spy.role())->spyCoins(spy, target);

    CHECK(target.arrestBlocked());
    CHECK_EQ(spy.coins(), 0);
    CHECK_EQ(target.coins(), 5);
}

// --------------------------------------------------------------
// Baron
// --------------------------------------------------------------
TEST_CASE("Baron: invest 3 → +6 net, sanction comp +1") {
    Player baron = makePlayer("Baron", new Baron{},      5);
    Player foe   = makePlayer("Foe",   new DummyRole{}, 10);

    static_cast<Baron*>(baron.role())->invest(baron);    // net +3
    CHECK(baron.coins() == 8);

    foe.sanction(baron);                                // foe pays 3
    static_cast<Baron*>(baron.role())->blockSanction(baron, foe);

    CHECK(baron.coins() == 9);                          // +1 comp
    CHECK(foe.coins()   == 7);
}

// --------------------------------------------------------------
// General
// --------------------------------------------------------------
TEST_CASE("General: blockCoup self, pay 5 & refund attacker") {
    Player general  = makePlayer("G",  new General{}, 6);
    Player attacker = makePlayer("A",  new DummyRole{}, 10);

    attacker.coup(general);                       // pays 7
    CHECK_FALSE(general.isActive());

    static_cast<General*>(general.role())->blockCoup(general, attacker);
    general.revive();                             // emulate Game’s revive

    CHECK(general.coins()  == 1);  // 6‑5
    CHECK(general.isActive());
}

// --------------------------------------------------------------
// Judge
// --------------------------------------------------------------
TEST_CASE("Judge: blockBribe sets flag, blockSanction penalises attacker") {
    Player judge   = makePlayer("J", new Judge{});
    Player briber  = makePlayer("B", new DummyRole{}, 6);

    briber.bribe();                               // pays 4
    static_cast<Judge*>(judge.role())->blockBribe(judge, briber);

    CHECK(briber.bribeBlocked());

    Player sanctioner = makePlayer("S", new DummyRole{}, 5);
    sanctioner.sanction(judge);                   // pays 3
    static_cast<Judge*>(judge.role())->blockSanction(judge, sanctioner);

    CHECK(sanctioner.coins() == 1);               // extra 1 paid
}

// --------------------------------------------------------------
// Merchant
// --------------------------------------------------------------
TEST_CASE("Merchant: blockArrest pays 2 to pot") {
    Player merch = makePlayer("M", new Merchant{}, 4);
    Player thief = makePlayer("T", new DummyRole{}, 1);

    thief.arrest(merch);                           // thief +1
    static_cast<Merchant*>(merch.role())->blockArrest(merch, thief);

    CHECK(merch.coins() == 2);                     // 4‑2
    CHECK(thief.coins() == 1);                     // unchanged
}