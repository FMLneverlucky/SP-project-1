#pragma once
#include "Entity.h"
class NPC :
    public Entity
{
private:
    bool angry;
    bool dead;
    int counter;
    float speed;
    int damage;
    
public:
    NPC();
    ~NPC();
    bool isHostile();
    void anger();
    void set_count(int a);
    int get_count();
    char type();  // return char to tell what type of entity this is
    float get_speed();
    void set_speed(float spd);
};

