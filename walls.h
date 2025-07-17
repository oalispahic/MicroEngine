//
// Created by Omar Alispahic on 17. 7. 2025..
//

#ifndef MICROENGINE_WALLS_H
#define MICROENGINE_WALLS_H

class Wall{
private:
    int xPos,yPos;
    bool collidable;
public:
    Wall(int xPos, int yPos, bool collidable): xPos(xPos), yPos(yPos), collidable(collidable){}

    bool isCollidable() const {return collidable;}

    virtual int getX() const {return xPos;}

    virtual int getY() const {return yPos;}

    virtual char draw() const = 0;


};

class SolidWall : public Wall{

public:
    SolidWall(int xPos, int yPos): Wall(xPos,yPos,true){}

    char draw() const override {
        return '#';
    }

};

class GhostWall : public Wall{
public:
    GhostWall(int xPos, int yPos): Wall(xPos,yPos,false){}

    char draw() const override{
        return '+';
    }
};

#endif //MICROENGINE_WALLS_H
