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
    Position wallPivotPoint;
    types nextWall;

public:
    Wall();
    ~Wall();
    char type();
    int setType();
    void update_pos(float dtime);
    Position* new_pos(float dtime);
};