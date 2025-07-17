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
    std::pair<int, int> *walls = new std::pair<int, int>[posS]; //Stores walls positions as a pair
    bool easyWalls = false; //Flag to toggle wall borders
public:
    Game(int mapX, int mapY) : mapX(mapX),
                               mapY(mapY) { //Constructor to instantiate the game object with map size
        this->playerPosX = 1;
        this->playerPosY = 1;
    }

    ~Game() {
        delete[] walls;    //We all know what this does lol
    }

    void render();

    static void clearScreen() {
        std::cout << "\033[H" << std::flush; //Used to clear the screen on terminals
    }

    void walk(int steps = 1); //Manual walk method for testing might be used later


    bool validPos(); //Bound checking method

    void move(char dir);

    void setBufferedInput(bool enable); //Method for making the terminal output work like a game

};

void Game::render() {
    clearScreen();
    std::cout<<'\n';
    std::cout<<"Wall maker V1.0 :)"<<std::endl;
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
            for (int k = 0; k < currentPos; k++) {                           //Loop for the trail
                if (walls[k].first == i && walls[k].second == j) { //Goes over the whole array each render cycle
                    isWall = true;                                        //checks if there are any coordinates that are
                    break;                                                 // a valid trail
                }
            }
            if (isWall) {
                std::cout << "# ";
            } else std::cout << "  ";
        }
        std::cout << std::endl;

    }
    std::cout<<std::boolalpha;
    std::cout<<"Easy walls toggle: "<<easyWalls<<'\n';
    std::cout<<"Walls placed: "<<currentPos;
    std::cout<<std::endl;
    std::cout<<"Controls: WASD -> up/left/down/right"<<'\n';
    std::cout<<"F -> place wall"<<'\n';
    std::cout<<"R -> toggle easy walls";


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
        if(easyWalls) return true;
        if (playerPosX == walls[i].first && playerPosY == walls[i].second) return false;
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
            auto newPos = std::make_pair(playerPosX, playerPosY);
            walls[currentPos] = newPos;
            currentPos++;
            break;
        }
        case 'r':{
            if(easyWalls) easyWalls = false;
            else easyWalls=true;
            render();
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

#endif
