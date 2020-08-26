#pragma once
#include "Entity.h"
class CCTV :
    public Entity
{
private:
    Position radar[20]; //position of CCTV's radar/line of sight
    bool cw; //rotation direction (clockwise/anticlockwise)
    int rotateCD; //no. of frames before rotation

public:
    CCTV();
    CCTV(int dir, bool cw); //constructor where user chooses intitial direction and rotation
    void setCD(double count);  //sets rotateCD
    int getCD(); //returns rotateCD
    void update_cctv(); //sets new direction and positions(of line of sight of cctv) accordingly
    Position* getRadarPos(int index); //function to return radar position
    char type();

    //pure virtual functions from Entity;;empty functions
    void update_pos(double dtime);
    Position* new_pos(double dtime); 

};
