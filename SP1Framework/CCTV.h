#pragma once
#include "Entity.h"
class CCTV :
    public Entity
{
private:
    Position radar[20];
    bool cw;
    int rotateCD; //no. of frames before rotation
public:
    CCTV();
    CCTV(int dir, bool cw); //constructor where user chooses intitial direction and rotation
    void setCD(double count);
    int getCD();
    void update_cctv(); //function to set new direction and positions(of line of sight of cctv) accordingly
    Position* getRadarPos(int index); //function to return radar position
    char type();

    //pure virtual functions from Entity;;empty functions
    void update_pos(double dtime);
    Position* new_pos(double dtime); 

};

