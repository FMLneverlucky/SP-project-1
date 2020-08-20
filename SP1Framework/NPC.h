#pragma once
#include "Entity.h"
class NPC :
    public Entity
{
private:
    bool angry;
    bool dead;
    int freezetime;
    int damage;
    float speed;

    int counter;
protected:
    int cooldown;
    
public:
    NPC(int cd);
    NPC();
    ~NPC();
    bool isHostile();
    void anger();
    void set_count(int a);
    int get_count();
    virtual char type();  // return char to tell what type of entity this is
    float get_speed();
    int get_damage();
    int get_ftime();
    void set_speed(float spd);
    void update_pos(float dtime);
    void cooldownstart();
    void cooldownend();
    Position* new_pos(float dtime);

};

