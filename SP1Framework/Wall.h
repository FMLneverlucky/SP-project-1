#pragma once
#include "Entity.h"

enum types
{
    I,
    J,
    L,
    O,
    S,
    T,
    Z,
    d
};

class Wall :
    public Entity

{
protected:
    Position wallPivot, wall2, wall3, wall4;
    types nextWall;

public:
    Wall();
    ~Wall();
    char type();
    void setPos(int x, int y);
    void setPosForAll();
    Position* getPos(int x);

    //updates for walls in game as entity
    void update_pos(double dtime);
    Position* new_pos(double dtime);
};