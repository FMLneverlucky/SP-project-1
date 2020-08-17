#pragma once
#include"Position.h"
class Object
{
	int sizeX;
	int sizeY;
	Position pos;
	Position pivot;
	int scale;
public:
	Object();//int, int, Position, Position);
	~Object();
};
