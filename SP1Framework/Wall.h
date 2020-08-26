#pragma once
#include "Entity.h"

class Wall :
    public Entity

{
protected:
    Position wallPos[4];

public:
    Wall();
    ~Wall();
    
    void setPos(int x, int y); //setspos of wallpivot/main wall aka this pos
    void setPosForAll(); //sets pos for other 3 walls depending on wall type and pos of mainwall
    Position* getPos(int x); //returns position of chosen wall(x representing wall index)
    char type();

    //updates for walls in game as entity
    void update_pos(double dtime);
    Position* new_pos(double dtime);
};