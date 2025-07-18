#ifndef MICROENGINE_GAME_H
#define MICROENGINE_GAME_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <utility>
#include <map>
#include "walls.h"

void delay(int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

class Game {

private:

    int playerPosX = 1; //Player coordinate X

    int playerPosY = 1; //Player coordinate Y

    int mapX, mapY;     //Map dimensions 

    int posS = (mapX - 2) * (mapY - 2); //Possible number of positions (a matrix without the borders)

    int wallsPlaced = 0; //Number of walls 

    Wall **wallArray = nullptr;

    bool wallsExist = false;    //Flag for rendering the walls

    enum class wallTypes {
        SolidWall, GhostWall, Count
    };

    int currentWall = 1;

    const char *returnWallName(wallTypes wall);

public:

    Game(int mapX, int mapY) : mapX(mapX), mapY(mapY) {}

    ~Game() { delete[] wallArray; }

    void render();

    static void clearScreen();

    void walk(int steps = 1); //Manual walk in x direction

    bool validPos(); //Bound checking method

    void move(char dir);

    void setBufferedInput(bool enable); //Method for making the terminal output work like a game

    void deleteWalls();

};

void Game::render() {
    clearScreen();
    std::cout << '\n';
    std::cout << "Wall maker V1.0 " << std::endl;
    for (int i = 0; i < mapX; i++) {
        for (int j = 0; j < mapY; j++) {

            if (i == 0 || i == mapX - 1 || j == 0 || j == mapY - 1) {     //Border draw
                std::cout << "* ";
                continue;
            }

            if (playerPosX == i && playerPosY == j) {       //Player position draw
                std::cout << "¡ ";
                continue;
            }

            bool isWall = false;
            for (int k = 0; k < wallsPlaced; k++) {
                if (wallArray[k]->getX() == i && wallArray[k]->getY() == j &&   //Loop for the walls
                    wallsPlaced > 0) {
                    wallArray[k]->draw();                   //Goes over the whole array each render cycle
                    isWall = true;                         //checks if there are any coordinates that are
                    break;                                // a valid wall
                }
            }
            if (!isWall) {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;

    }
    std::cout << "Walls placed: " << wallsPlaced;
    std::cout << std::endl;
    std::cout << "Controls: WASD -> up/left/down/right" << '\n';
    std::cout << "F -> Place wall" << '\n';
    std::cout << "C -> Clear all the walls" << '\n';
    std::cout << "Q -> Quit the game" << '\n';
    std::cout << "SPACE -> Toggle wall" << '\n';
    std::cout << "Selected wall type: " << returnWallName(wallTypes(currentWall)) << '\n';
}

void Game::walk(int steps) {
    playerPosX += steps;
    if (!validPos()) {
        playerPosX -= 1;
        std::cout << "Player tried to escape!" << std::endl;
    }
}

bool Game::validPos() {
    if (playerPosX == mapX - 1 || playerPosX == 0 || playerPosY == mapY - 1 || playerPosY == 0) {
        return false;
    }
    for (int i = 0; i < wallsPlaced; i++) {

        if (playerPosX == wallArray[i]->getX() && playerPosY == wallArray[i]->getY()) {
            if (wallArray[i]->isCollidable()) return false;
        }
    }
    return true;
}

void Game::move(char dir) {
    int oldX = playerPosX;
    int oldY = playerPosY;
    switch (dir) {
        case 'w':
            playerPosX--;
            break;
        case 'a':
            playerPosY--;
            break;
        case 's':
            playerPosX++;
            break;
        case 'd':
            playerPosY++;
            break;
        case 'f': {
            if (!wallsExist) {
                wallArray = new Wall *[posS];
                wallsExist = true;
            }
            switch(currentWall){
                case 0:{
                    auto newWall = new SolidWall(playerPosX, playerPosY);
                    wallArray[wallsPlaced] = newWall;
                    wallsPlaced++;
                }
                case 1:{
                    auto newWall = new GhostWall(playerPosX, playerPosY);
                    wallArray[wallsPlaced] = newWall;
                    wallsPlaced++;
                }
                break;
            }
            break;
        }

        case 'c': {
            deleteWalls();
            render();
            break;
        }
        case ' ': {
            currentWall++;
            currentWall %= static_cast<int>(wallTypes::Count);
            break;
        }
    }
    if (!validPos()) {
        playerPosX = oldX;
        playerPosY = oldY;
    }

}

void Game::setBufferedInput(bool enable) {
    static bool initialized = false;
    static struct termios old;
    struct termios newt;

    if (!initialized) {
        tcgetattr(STDIN_FILENO, &old);
        initialized = true;
    }

    newt = old;
    newt.c_lflag &= ~(ICANON | ECHO);
    if (enable) newt = old;

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void Game::deleteWalls() {
    delete[] wallArray;
    wallsPlaced = 0;
    wallArray = nullptr;
    wallsExist = false;
}

void Game::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

const char *Game::returnWallName(Game::wallTypes wall) {
    const char *wallName;
    switch (wall) {
        case wallTypes::SolidWall: {
            wallName = "Solid wall";
            break;
        }
        case wallTypes::GhostWall: {
            wallName = "Transparent wall";
            break;
        }
         default: {
            return "??";
        }
    }
    return wallName;
}

#endif
