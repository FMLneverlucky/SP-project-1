#pragma once
#include "Entity.h"
class Player : public Entity
{
private:
    int HP;
    int cooldown; //coughs

public:
    Player();
    ~Player();
    char type();  // return char to tell what type of entity this is
    int get_HP();
    void resetHP();
    void loseHP(int ack);
    void update_pos(float dtime);
    void set_cooldown(int a);
    int get_cooldown();
    Position* new_pos(float dtime);
};

