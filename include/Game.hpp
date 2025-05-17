// alaasalah142@gmail.com
// include/Game.hpp
#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <initializer_list>
#include "Player.hpp"
#include "roles/Merchant.hpp"   // for dynamic_cast
#include "roles/General.hpp"
#include "roles/Governor.hpp"

class Game {
private:
    std::vector<Player> _players;   // players in join order
    size_t              _turnIdx;   // index of current player
    std::vector<int>    _lastArrested; // last arrest target per player (−1 if none)

    // --- Helpers ----------------------------------------------------------
    void          startTurnEffects();
    void          advanceTurn();

    // Mandatory‑coup rule: true if active player has ≥10 coins at start of turn
    bool mustCoup() const;

public:
    // -- Helpers -----------------------------------------------------------
    Player&       active();          // current player (mutable)
    const Player& active() const;    // current player (const)
    size_t        turnIdx();
    const size_t  turnIdx() const;

    // Construct with a list of players (copies them)
    explicit Game(std::initializer_list<Player> players);

    // --- Queries ----------------------------------------------------------
    const std::string& turn()   const;            // name of active player
    std::vector<std::string>   players() const;   // active players’ names
    std::string               winner()  const;   // throws if >1 active

    // --- Action wrappers (to throw exception in case of must coup) --------
    void gather();                  // +1 coin (blockable)
    void tax();                     // +2 coins (blockable / Governor variant)
    void bribe();                   // −4 coins (enables second action; blockable)
    void arrest(size_t targetIdx);  // steal 1 coin (restrictions apply)
    void sanction(size_t targetIdx);// pay 3; block economic actions for target
    void coup(size_t targetIdx);    // pay 7; eliminate target (blockable by General)

    /// Baron-only: invest action (counts as your turn)
    void invest();

    /// Spy-only: spyCoins on target (does *not* advance turn)
    void spy(size_t targetIdx);

    // Called out‑of‑turn by a General to block a coup that just targeted someone
    void generalBlocksCoup(size_t blockerIdx, size_t actorIdx, size_t targetIdx);

    // In-turn action by Governor to block someone's tax
    void governorBlocksTax(size_t governorIdx, size_t taxedPlayerIdx);

    // Return the total number of players in this game (initial size).
    size_t playerCount() const;

    // Get a reference to the i-th player (0≤i<playerCount()).
    // Throws std::out_of_range if i is invalid.
    const Player& getPlayer(size_t i) const;
};