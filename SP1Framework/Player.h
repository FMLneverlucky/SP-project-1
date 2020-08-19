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
    int get_HP();
    void resetHP();
    void loseHP(int ack);
    void update_pos(float dtime);
    Position* new_pos(float dtime);
};

