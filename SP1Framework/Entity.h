#pragma once
#include "Position.h"

enum direction
{
	NOT,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Entity
{
private:
	direction direction = NOT;
	Position pos;
public:
	Entity();
	~Entity();
	void set_direction(int a);
	void set_pos();
	void set_pos(int xx, int yy);
	int getposx();
	int getposy();
};

