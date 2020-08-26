#pragma once
#include "Position.h"

class PowerUp
{
private:
	Position UPpos;
	int detime; //despawn time
public:
	PowerUp();
	~PowerUp();
	void set_xcoord(float a);
	void set_ycoord(float b);
	float get_xcoord();
	float get_ycoord();
	void set_detime(int d);
	int get_detime(); 
	Position* get_pos();
};