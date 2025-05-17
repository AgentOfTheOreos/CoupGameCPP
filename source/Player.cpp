// alaasalah142@gmail.com
// src/Player.cpp
#include "../include/Player.hpp"
#include "../include/Role.hpp"

// Constructor: takes ownership of role pointer
Player::Player(const std::string& name, Role* role)
    : _name(name), _role(role), _coins(0), _isActive(true), _arrestBlocked(false), _taxBlocked(false), _bribeBlocked(false), _sanctionActive(false) {
    if (!_role) {
        throw std::invalid_argument("Role pointer cannot be null");
    }
}

// Copy constructor: deep-clone the Role
Player::Player(const Player& other)
    : _name(other._name),
      _role(other._role ? other._role->clone() : nullptr),
      _coins(other._coins),
      _isActive(other._isActive),
      _arrestBlocked(other._arrestBlocked),
      _taxBlocked(other._taxBlocked),
      _bribeBlocked(other._bribeBlocked),
      _sanctionActive(other._sanctionActive) {
    if (! _role) {
        throw std::runtime_error("Failed to clone Role in Player copy constructor");
    }
}

// Copy assignment operator: clone-and-swap-like for exception safety
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        _name = other._name;
        // Clone first to ensure strong exception guarantee
        Role* newRole = other._role ? other._role->clone() : nullptr;
        if (! newRole) {
            throw std::runtime_error("Failed to clone Role in Player assignment");
        }
        delete _role;
        _role = newRole;
        _coins    = other._coins;
        _isActive = other._isActive;
        _arrestBlocked = other._arrestBlocked;
        _taxBlocked = other._taxBlocked;
        _bribeBlocked = other._bribeBlocked;
        _sanctionActive = other._sanctionActive;
    }
    return *this;
}

// Destructor: clean up owned Role
Player::~Player() {
    delete _role;
}

// Action delegates
void Player::gather()                           { _role->gather(*this); }
void Player::tax()                              { _role->tax(*this); }
void Player::bribe()                            { _role->bribe(*this); }
void Player::arrest(Player& target)             { _role->arrest(*this, target); }
void Player::sanction(Player& target)           { _role->sanction(*this, target); }
void Player::coup(Player& target)               { _role->coup(*this, target); }

// Coin management
void Player::addCoins(int n) {
    if (n < 0) {
        throw std::invalid_argument("Cannot add negative coins");
    }
    _coins += n;
}

void Player::payCoins(int n) {
    if (n < 0) {
        throw std::invalid_argument("Cannot pay negative coins");
    }
    if (_coins < n) {
        throw std::runtime_error("Not enough coins");
    }
    _coins -= n;
}

bool Player::hasCoins(int n) const {
    return _coins >= n;
}

// Setters
void Player::setArrestBlockedFlag(bool flag)    { _arrestBlocked = flag; }
void Player::setTaxBlockedFlag(bool flag)       { _taxBlocked = flag; }
void Player::setBribeBlockedFlag(bool flag)     { _bribeBlocked = flag; }
void Player::setSanctionFlag(bool flag)         { _sanctionActive = flag; }
void Player::extraAction(bool flag)      { _extraAction = flag; }

// Accessors and state changes
Role* Player::role() const                      { return _role; }
const std::string& Player::name() const         { return _name; }
int Player::coins() const                       { return _coins; }
bool Player::isActive() const                   { return _isActive; }
bool Player::arrestBlocked() const              { return _arrestBlocked; }
bool Player::taxBlocked() const                 { return _taxBlocked; }
bool Player::bribeBlocked() const               { return _bribeBlocked; }
bool Player::sanctionActive() const             { return _sanctionActive; }
bool Player::hasExtraAction() const             { return _extraAction; }

void Player::eliminate() {
    _isActive = false;
}

void Player::resetFlags() {
    _arrestBlocked = false;
    _taxBlocked = false;
    _bribeBlocked = false;
    _sanctionActive = false;
}

void Player::revive() {
    _isActive = true;
    resetFlags();
}