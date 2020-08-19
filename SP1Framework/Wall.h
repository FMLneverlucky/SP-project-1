#pragma once
#include "Entity.h"

enum types
{
    red,
    orange,
    yellow,
    green,
    light_blue,
    blue,
    purple,
};

class Wall :
    public Entity

{
private:
    types wall;

public:
    Wall();
    ~Wall();
    char type();
    void update_pos(float dtime);
    Position* new_pos(float dtime);
};