#include "game.h"
#include <iostream>

int main() {
    int x, y;
    std::cout << "Set map size x,y:";
    std::cin >> x >> y;
    Game g1(x, y);
    g1.setBufferedInput(false);
    char input;
    Game::clearScreen();
    g1.render();



    while (true) {
        if (read(STDIN_FILENO, &input, 1) == 1) {
            g1.commands(input);
            g1.render();

            if (input == 'q') break;
        }
        delay(25);
    }
    Game::clearScreen();

    return 0;
}