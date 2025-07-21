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

    Wall() = default;

    bool isCollidable() const {return collidable;}

    virtual int getX() const {return xPos;}

    virtual int getY() const {return yPos;}

    virtual void draw() const = 0;

};

class SolidWall : public Wall{
public:
    SolidWall(int xPos, int yPos): Wall(xPos,yPos,true){}

    void draw() const override {
        std::cout<<"# ";


    }
};

class GhostWall : public Wall{
public:
    GhostWall(int xPos, int yPos): Wall(xPos,yPos,false){}

    void draw() const override{
        std::cout<<"+ ";
    }
};

#endif //MICROENGINE_WALLS_H
