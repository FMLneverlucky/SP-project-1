#pragma once
#include "Position.h"

class Zone
{
private:
	Position pos[9];
public:
	void setpos(int x, int y);
	Position* getpos(int index);
};

