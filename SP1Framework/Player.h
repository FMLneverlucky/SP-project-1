#pragma once
#include "Entity.h"
class Player : public Entity
{
private:
    int HP; //current HP of player
    int maxHP; //max HP of player
    bool flash; //whether player is flashing or not
    bool lethal;
    int lethality_duration;

public:
    Player();
    ~Player();
    
    //Getters
    int get_HP();
    int get_maxHP();
    bool get_flash();
    int get_lethalstatus();

    //Setters
    void resetHP(); //sets HP to maxHP
    void loseHP(int ack); //reduce HP by ack(damage of npc passed in)
    void set_flash(bool what); //sets flash to true/false
    Position* new_pos(double dtime); //returns new position of player before its position is set
    void update_pos(double dtime); //moves player by no of squares per frame
    void set_lethal();
    void update_ld(); //lethality duration timer ish
    void resetlethality();
    
    char type();  // return char to tell what type of entity this is

};

