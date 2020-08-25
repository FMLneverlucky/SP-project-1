#pragma once
#include "Position.h"

class PowerUp
{
private:
	Position UPpos;
public:
	PowerUp();
	~PowerUp();
	void set_xcoord(float a);
	void set_ycoord(float b);
	float get_xcoord();
	float get_ycoord();
	Position* get_pos();
};