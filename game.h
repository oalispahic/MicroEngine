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
    int playerPosX, playerPosY; //Real player position in the map

    int mapX, mapY;     //Map size given as a matrix

    int posS = (mapX - 2) * (mapY - 2); //Possible number of positions (a matrix without the borders)

    int currentPos = 0; //This is only used for rendering the player "trail" indicator of how many trails in array


    Wall **wallArray = nullptr;


    bool wallsExist = false;
public:
    Game(int mapX, int mapY) : mapX(mapX),
                               mapY(mapY) { //Constructor to instantiate the game object with map size
        this->playerPosX = 1;
        this->playerPosY = 1;
    }

    ~Game() {
        delete[] wallArray;    //We all know what this does lol
    }

    void render();      //Makes the game come to life

    static void clearScreen() {
        //std::cout << "\033[H" << std::flush; //Used to clear the screen on terminals
        std::cout << "\033[2J\033[H" << std::flush;
    }

    void walk(int steps = 1); //Manual walk method for testing might be used later


    bool validPos(); //Bound checking method

    void move(char dir);

    void setBufferedInput(bool enable); //Method for making the terminal output work like a game

    void deleteWalls();

};

void Game::render() {
    clearScreen();
    std::cout << '\n';
    std::cout << "Wall maker V1.0 :)" << std::endl;
    for (int i = 0; i < mapX; i++) {
        for (int j = 0; j < mapY; j++) {

            if (i == 0 || i == mapX - 1 || j == 0 || j == mapY - 1) {     //Border draw
                std::cout << "* ";
                continue;
            }

            if (playerPosX == i && playerPosY == j) {       //Player position draw
                std::cout << "! ";
                continue;
            }

            bool isWall = false;
            for (int k = 0; k < currentPos; k++) {
                if (wallArray[k]->getX() == i && wallArray[k]->getY() == j &&             //Loop for the trail
                    currentPos > 0) {
                    wallArray[k]->draw();                          //Goes over the whole array each render cycle
                    isWall = true;                                           //checks if there are any coordinates that are
                    break;                                                  // a valid trail
                }
            }
            if (!isWall) {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;

    }
    std::cout << "Walls placed: " << currentPos;
    std::cout << std::endl;
    std::cout << "Controls: WASD -> up/left/down/right" << '\n';
    std::cout << "F -> Place wall" << '\n';
    std::cout << "R -> Toggle easy walls" << '\n';
    std::cout << "C -> Clear all the walls" << '\n';
    std::cout << "Q -> Quit the game" << '\n';


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
    for (int i = 0; i < currentPos; i++) {
        if(playerPosX == wallArray[i]->getX() && playerPosY == wallArray[i]->getY()){
            if(wallArray[i]->isCollidable()) return false;
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
                //walls = new std::pair<int, int>[posS];
                wallArray = new Wall *[posS];
                wallsExist = true;
            }
            //auto newPos = std::make_pair(playerPosX, playerPosY);
            SolidWall *newWall = new SolidWall(playerPosX, playerPosY);
            wallArray[currentPos] = newWall;
            currentPos++;
            break;
        }

        case 'c': {
            deleteWalls();
            render();
            break;
        }
        case 'g':{
            if (!wallsExist) {
                //walls = new std::pair<int, int>[posS];
                wallArray = new Wall *[posS];
                wallsExist = true;
            }
            //auto newPos = std::make_pair(playerPosX, playerPosY);
            GhostWall *newWall = new GhostWall(playerPosX, playerPosY);
            wallArray[currentPos] = newWall;
            currentPos++;
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
    currentPos = 0;
    wallArray = nullptr;
    wallsExist = false;
}

#endif
