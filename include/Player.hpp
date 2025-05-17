// alaasalah142@gmail.com
// include/Player.hpp
#pragma once

#include "Role.hpp"
#include <string>
#include <stdexcept>

class Role;

class Player {
private:
    std::string _name;
    Role*       _role;
    int         _coins;
    bool        _isActive;
    bool        _arrestBlocked;
    bool        _taxBlocked;
    bool        _bribeBlocked;
    bool        _sanctionActive;
    bool        _extraAction;

public:
    // Constructor takes ownership of a Role*
    Player(const std::string& name, Role* role);

    // Rule of Three
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    // Actions (delegate to Role)
    void gather();
    void tax();
    void bribe();
    void arrest(Player& target);
    void sanction(Player& target);
    void coup(Player& target);
    void invest() { _role->invest(*this); }
    void spyCoins(Player& target) { _role->spyCoins(*this, target); }

    // Role implementations will use these
    void addCoins(int n);
    void payCoins(int n);
    bool hasCoins(int n) const;

    // Setters
    void setArrestBlockedFlag(bool flag);
    void setTaxBlockedFlag(bool flag);
    void setBribeBlockedFlag(bool flag);
    void setSanctionFlag(bool flag);
    void extraAction(bool flag);
    
    // Accessors and state changes
    Role* role() const;
    const std::string& name() const;
    int coins() const;
    bool isActive() const;
    bool arrestBlocked() const;
    bool taxBlocked() const;
    bool bribeBlocked() const;
    bool sanctionActive() const;
    bool hasExtraAction() const;
    void eliminate();
    void resetFlags();
    void revive();
};