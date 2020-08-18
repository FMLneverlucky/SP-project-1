#pragma once
#include "Entity.h"
class NPC :
    public Entity
{
private:
    bool angry;
    bool dead;
    int counter = 0;
    
public:
    NPC();
    ~NPC();
    bool isHostile();
    void anger();
    void set_count(int a);
    int get_count();
    
};

