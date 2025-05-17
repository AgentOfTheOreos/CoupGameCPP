# Coup Demo

A compact C++ implementation of key object-oriented concepts (Rule of Three, inheritance, polymorphism) in the form of a simplified digital version of the board game **Coup**, with a minimal SFML GUI.

---

## Features & Concepts

- **Rule of Three**: `Player` manages a polymorphic `Role*`, implements copy-ctor, copy-assignment, and destructor.  
- **Abstract Base Class & Polymorphism**: `Role` defines core and blockable actions; subclasses (`Governor`, `Spy`, `Baron`, `General`, `Judge`, `Merchant`) override only their special behaviors.  
- **Game Engine**: `Game` manages turn order, enforces “must coup at ≥ 10 coins,” handles arrest/sanction/coup mechanics, and coordinates out-of-turn reactions (General’s coup block, Governor’s delayed tax block).  
- **Flags & Sanctions**: Per-player flags (`arrestBlocked`, `taxBlocked`, `bribeBlocked`, `sanctionActive`) that last until the player actually attempts an action.  
- **Unit Testing**: Doctest suites cover `Player`, `Role` subclasses, core game logic, and cross-role blocking.  
- **Memory Safety**: All tests and the demo can be run under Valgrind to ensure zero leaks.

---

## Requirements

- A C++23-compliant compiler (tested with Clang++ and MinGW-w64 GCC on MSYS2)  
- [SFML](https://www.sfml-dev.org/) (graphics, window, system modules)  
- [Doctest](https://github.com/doctest/doctest) (header-only, included under `tests/`)  
- (Optional) Valgrind for `memcheck-demo` / `memcheck-tests` on Linux/macOS

---

## Repository Layout

```

.
├── include/
│   ├── misc/
│   │   └── DummyRole.hpp
│   ├── Role.hpp
│   ├── Player.hpp
│   ├── Game.hpp
│   ├── GUI.hpp
│   └── roles/       ← concrete Role subclasses
│       ├── Governor.hpp
│       ├── Spy.hpp
│       ├── Baron.hpp
│       ├── General.hpp
│       ├── Judge.hpp
│       └── Merchant.hpp
├── source/
│   ├── Role.cpp
│   ├── Player.cpp
│   ├── Game.cpp
│   ├── GUI.cpp
│   └── roles/       ← Role subclasses CCP's
│       ├── Governor.cpp
│       ├── Spy.cpp
│       ├── Baron.cpp
│       ├── General.cpp
│       ├── Judge.cpp
│       └── Merchant.cpp
├── tests/
│   ├── test\_player.cpp
│   ├── test\_roles.cpp
│   └── test\_game\_core.cpp
├── assets/
│   └── arial.ttf    ← GUI font
├── Makefile
├── main.cpp
└── README.md

````

---

## Build & Run

1. **Install dependencies**  
   - On Ubuntu/Debian:
     ```bash
     sudo apt update
     sudo apt install build-essential clang++ libsfml-dev valgrind
     ```
   - On MSYS2 (64-bit):
     ```bash
     pacman -Syu
     pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-SFML
     ```
2. **Clone & Enter**  
   ```bash
   git clone <repo-url> && cd cppex03

3. **Build everything**

   ```bash
   make all
   ```
4. **Run Tests**

   ```bash
   make tests
   ./test_player
   ./test_roles
   ./test_game_core
   ./test_blocks
   ```
5. **Run Demo**

   ```bash
   make demo
   ./demo
   ```

   * Use keys **1–8** to invoke actions:

     1. Gather  2) Tax  3) Bribe  4) Invest (Baron)
     2. Arrest  6) Sanction  7) Coup  8) Spy
   * For actions 5–7 and spy (8), follow the console prompt to choose a target index.
6. **Memory Check (Linux/macOS)**

   ```bash
   make memcheck-demo     # Demo under Valgrind
   make memcheck-tests    # All test suites under Valgrind
   ```

---

## Gameplay Controls & UI Notes

* **Active player’s coins** are visible; all others display `?? coins` until spied upon.
* **Sidebar** shows each player’s:

  * Index, Name, Role
  * Coin count or `??`
  * Status flags: `[ELIM]`, `[NoArrest]`, `[NoTax]`, `[NoBribe]`, `[Sanction]`
* **Action menu** is greyed out for role-ineligible options (e.g., Invest only for Baron).
* **Mandatory Coup**: if you hold ≥ 10 coins, only the Coup action is allowed.
* **Block reactions**:

  * **General** can intercept a Coup (pay 5) to revive the target and refund the attacker.
  * **Governor** can cancel a previous Tax out of turn.
  * **Spy** can peek another’s coins and block their Arrest next turn.
  * **Baron**, **Judge**, **Merchant** each have their own block/compensation rules.

---

## License

This project is provided “as is” for educational purposes. Feel free to adapt and extend.
Author: Aladdin Abu Hegly  |   alaasalah142@gmail.com