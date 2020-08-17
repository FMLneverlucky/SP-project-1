#pragma once
#include "Entity.h"
class NPC :
    public Entity
{
private:
    bool angry;
    bool dead;
    
public:
    NPC();
    ~NPC();
    bool isHostile();
    
};

