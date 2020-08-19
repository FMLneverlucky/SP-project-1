#pragma once
#include "Entity.h"
class Player : public Entity
{
private:
    int HP;
public:
    Player();
    ~Player();
    char type();  // return char to tell what type of entity this is
    void update_pos();
};

