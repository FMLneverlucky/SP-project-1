#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
    Player();
    ~Player();
    char type();  // return char to tell what type of entity this is
};

