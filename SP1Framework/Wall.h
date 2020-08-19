#pragma once
#include "Entity.h"

class Wall:
    public Entity
{
private:

public:
    Wall();
    ~Wall();
    char type();
    void update_pos(float dtime);
    Position* new_pos(float dtime);
};