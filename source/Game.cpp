// alaasalah142@gmail.com
// src/Game.cpp
#include "../include/Game.hpp"
#include <algorithm>

// ---------------- Helper accessors ----------------
Player& Game::active() {
    return _players[_turnIdx];
}
const Player& Game::active() const {
    return _players[_turnIdx];
}

size_t Game::turnIdx() {
    return _turnIdx;
}

const size_t Game::turnIdx() const {
    return _turnIdx;
}
bool Game::mustCoup() const {
    return active().coins() >= 10; // 10 or more coins forces coup
}

// ---------------- Construction ----------------
Game::Game(std::initializer_list<Player> players)
    : _players(players), _turnIdx(0), _lastArrested(players.size(), -1) {
    if (_players.size() < 2) {
        throw std::invalid_argument("Game requires at least two players");
    }
    startTurnEffects();
}

// ---------------- Turn management --------------
void Game::startTurnEffects() {
    // Merchant passive ability
    if (dynamic_cast<const Merchant*>(active().role()) && active().coins() >= 3) {
        active().addCoins(1);
    }
}

void Game::advanceTurn() {
    if (_players.empty()) { return; }

    if(active().hasExtraAction()) {
        active().extraAction(false);
        return;
    }

    size_t n = _players.size();
    do {
        _turnIdx = (_turnIdx + 1) % n;
    } while (!_players[_turnIdx].isActive());
    startTurnEffects();
}

// ---------------- Queries ----------------------
const std::string& Game::turn() const {
    return active().name();
}

std::vector<std::string> Game::players() const {
    std::vector<std::string> names;
    for (const auto& p : _players) {
        if (p.isActive()) {
            names.push_back(p.name());
        }
    }
    return names;
}

std::string Game::winner() const {
    int count = 0;
    std::string lastName;
    for (const auto& p : _players) {
        if (p.isActive()) {
            ++count;
            lastName = p.name();
        }
    }
    if (count == 1) return lastName;
    throw std::runtime_error("Game is still in progress – no winner yet");
}

// ---------------- Action wrappers --------------
void Game::gather() {
    if (mustCoup()) throw std::runtime_error("Player must coup when holding 10 or more coins");
    active().gather();
    advanceTurn();
}

void Game::tax() {
    if (mustCoup()) throw std::runtime_error("Player must coup when holding 10 or more coins");
    active().tax();
    advanceTurn();
}

void Game::bribe() {
    if (mustCoup()) throw std::runtime_error("Player must coup when holding 10 or more coins");
    active().bribe();
    // For simplicity we count bribe as the turn’s action
    advanceTurn();
}

void Game::arrest(size_t targetIdx) {
    if (mustCoup()) throw std::runtime_error("Player must coup when holding 10 or more coins");
    if (targetIdx >= _players.size()) throw std::out_of_range("Invalid target index");
    if (!_players[targetIdx].isActive()) throw std::runtime_error("Target player already eliminated");
    size_t actorIdx = _turnIdx;
    if (_lastArrested[actorIdx] == static_cast<int>(targetIdx)) {
        throw std::runtime_error("Cannot arrest the same player twice in a row");
    }
    if (targetIdx == actorIdx) {
        throw std::runtime_error("Cannot arrest yourself");
    }
    active().arrest(_players[targetIdx]);
    _lastArrested[actorIdx] = static_cast<int>(targetIdx);
    advanceTurn();
}

void Game::sanction(size_t targetIdx) {
    if (mustCoup()) throw std::runtime_error("Player must coup when holding 10 or more coins");
    if (targetIdx >= _players.size()) throw std::out_of_range("Invalid target index");
    if (!_players[targetIdx].isActive()) throw std::runtime_error("Target player already eliminated");
    size_t actorIdx = _turnIdx;
    if (targetIdx == actorIdx) {
        throw std::runtime_error("Cannot sanction yourself");
    }
    active().sanction(_players[targetIdx]);
    advanceTurn();
}

void Game::coup(size_t targetIdx) {
    size_t actorIdx = _turnIdx;
    if (targetIdx >= _players.size()) throw std::out_of_range("Invalid target index");
    if (!_players[targetIdx].isActive()) throw std::runtime_error("Target player already eliminated");
    if (targetIdx == actorIdx) {
        throw std::runtime_error("Cannot coup yourself");
    }
    // Perform coup payment & elimination
    active().coup(_players[targetIdx]);
    advanceTurn();
}

void Game::invest() {
    if (mustCoup()) 
        throw std::runtime_error("Player must coup when holding 10+ coins");

    // Only the active player invests on themselves
    active().invest();
    advanceTurn();
}

void Game::spy(size_t targetIdx) {
    size_t actorIdx = _turnIdx;
    // no mustCoup check (spy is “free”)
    if (targetIdx >= _players.size()) 
        throw std::out_of_range("Invalid target for spy");
    if (!_players[targetIdx].isActive())
        throw std::runtime_error("Cannot spy on eliminated player");
    if (targetIdx == actorIdx)
        throw std::runtime_error("Cannot spy on yourself");

    active().spyCoins(_players[targetIdx]);
    // NOTE: spy does *not* advanceTurn() or consume extraAction
}

// ---------------- Coup blocking by General -----
void Game::generalBlocksCoup(size_t blockerIdx, size_t actorIdx, size_t targetIdx) {
    if (blockerIdx >= _players.size() ||
        actorIdx   >= _players.size() ||
        targetIdx  >= _players.size())
        throw std::out_of_range("Bad index");

    Player& blocker = _players[blockerIdx];
    Player& actor   = _players[actorIdx];
    Player& target  = _players[targetIdx];

    if (!blocker.isActive() || !actor.isActive() || target.isActive())
        throw std::runtime_error("Invalid coup state");

    // Verify the blocker really is a General
    auto* g = dynamic_cast<General*>(blocker.role());
    if (!g) throw std::runtime_error("Only a General can block a coup");

    // General pays 5 coins
    g->blockCoup(blocker, actor);

    // Refund the coup cost
    actor.addCoins(7);

    // Revive the couped player
    target.revive();
}

// ---------------- Tax blocking by Governor -----
void Game::governorBlocksTax(size_t govIdx, size_t taxedIdx)
{
    if (govIdx >= _players.size() || taxedIdx >= _players.size())
        throw std::out_of_range("Bad index");

    Player& gov   = _players[govIdx];
    Player& taxed = _players[taxedIdx];

    // Only living Governors may call this:
    auto* g = gov.isActive() ? dynamic_cast<Governor*>(gov.role()) : nullptr;
    if (!g) throw std::runtime_error("Caller is not an active Governor");

    // Must be called *after* the taxed player’s tax action but *before* that
    // player takes his next turn.  We don’t need extra bookkeeping—just apply it.
    g->blockTax(gov, taxed);
}

size_t Game::playerCount() const {
    return _players.size();
}

const Player& Game::getPlayer(size_t i) const {
    if (i >= _players.size()) throw std::out_of_range("Bad player index");
    return _players[i];
}