#pragma once
#include "Entity.h"
class Player : public Entity
{
private:
    int HP;
    int maxHP;
    int cooldown; //coughs
    bool flash = false;

public:
    Player();
    ~Player();
    char type();  // return char to tell what type of entity this is
    int get_HP();
    int get_maxHP();
    void resetHP();
    void loseHP(int ack);
    void update_pos(double dtime);
    void set_cooldown(int a);
    int get_cooldown();
    void set_flash(bool what);
    bool get_flash();
    Position* new_pos(double dtime);
};

