#pragma once
#include "Entity.h"

//enum types
//{
//
//};

class Wall :
    public Entity

{
private:
    //int type;

public:
    Wall();
    ~Wall();
    char type();
    void update_pos();
    Position* new_pos();
};