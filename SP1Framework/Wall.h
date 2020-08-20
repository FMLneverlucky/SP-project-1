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
    types nextWall;

public:
    Wall();
    ~Wall();
    char type();
    int setType();
    void Wall::typeI();
    void Wall::typeJ();
    void Wall::typeL();
    void Wall::typeO();
    void Wall::typeS();
    void Wall::typeT();
    void Wall::typeZ();
    void Wall::typed();
    void update_pos(float dtime);
    Position* new_pos(float dtime);
};