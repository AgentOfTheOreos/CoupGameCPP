// alaasalah142@gmail.com
#pragma once
#include "../Role.hpp"
#include "../Player.hpp"
// --------------------------------------------------------------
// Minimal concrete role used for “plain” behaviour tests
// --------------------------------------------------------------
struct DummyRole : Role { DummyRole* clone() const override { return new DummyRole(*this); } };

// Helper to make a Player with starting coins
static Player makePlayer(const std::string& name, Role* r, int start = 0)
{
    Player p(name, r);
    p.addCoins(start);
    return p;
}