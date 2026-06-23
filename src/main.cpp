#include "Game.h"
#include "state/MenuState.h"
#include "constants.h"
#include <cstdlib>
#include <ctime>

using namespace std;


int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;
    game.pushState(make_unique<MenuState>(game));
    game.run();
    return 0;
}
