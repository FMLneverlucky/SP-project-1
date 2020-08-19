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
protected:
	direction direction = NOT;
	Position pos;
	Position tempp;
	
public:
	Entity();
	~Entity();
	void set_direction(int a);
	virtual void update_pos() = 0;
	void set_pos(float xx, float yy);
	float getposx();
	float getposy();
	Position* getpos();
	virtual Position* new_pos() = 0;
	virtual char type() = 0; // return char to tell what type of entity this is
	
};

