// alaasalah142@gmail.com
#include "include/GUI.hpp"
#include "include/Player.hpp"
#include "include/roles/Governor.hpp"
#include "include/roles/Spy.hpp"
#include "include/roles/Baron.hpp"
#include "include/roles/General.hpp"

int main() {
    // sample players
    Player p1("Amit",    new Governor{});
    Player p2("Yossi",   new Spy{});
    Player p3("Dana",    new Baron{});
    Player p4("Michael", new General{});

    Game game{p1, p2, p3, p4};

    GUI gui(game);
    gui.run();

    return 0;
}
