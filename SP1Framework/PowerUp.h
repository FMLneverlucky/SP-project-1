#pragma once
#include "Position.h"

class PowerUp
{
private:
	Position pos;
	float timeleft; //before disappearing
public:
	PowerUp(float duration);
	PowerUp();
	
};