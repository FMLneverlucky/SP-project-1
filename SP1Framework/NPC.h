#pragma once
#include "Entity.h"
class NPC :
    public Entity
{
private:
    bool angry; //whether NPC is hostile or not
    int damage; //damage of NPC
    float speed; //speed of NPC relative to player when turned hostile
    float lifespan; //lifespan of NPCs in Endless mode
    static int noHostile; //total number of NPCs turned Hostile
    static int total; //total number of NPCs

    int counter; //cooldown time(in frames)

protected:
    bool onCD; //whether NPC is on cooldown or not
    float freezetime; //cooldown time(in seconds)

public:
    NPC(int cd); //allows user to choose cooldown time of NPC when spawning
    NPC();
    ~NPC();

    //Getters
    int get_count();
    int get_damage();
    float get_speed();
    float get_ftime();
    float get_lifespan();
    bool isonCD();

    //Setters
    void set_count(int a);
    void set_speed(float spd);
    void set_lifespan(float lifespan);

    bool isHostile(); //returns angry
    void anger(); //sets angry to true
    void update_pos(double dtime); //moves NPCs by no of sqrs per frame, depending on their direction
    void cooldownstart(); //sets onCD to true
    void cooldownend(); //sets onCD to false
    
    static int gettotal(); //returns total no. of NPCs
    static int getnoHostile(); //returns total no. of NPCs turned Hostile
    static void resetnoHostile(); //sets total no. of NPCs turned Hostile to 0
    
    Position* new_pos(double dtime); //returns new pos of NPC before moving in that frame
    virtual char type();  // return char to tell what type of entity this is
};

