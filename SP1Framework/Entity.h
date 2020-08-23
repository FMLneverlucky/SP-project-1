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
	Position rpos;
	
public:
	Entity();
	~Entity();
	void set_rpos(float x, float y);
	void set_direction(int a);
	virtual void update_pos(double dtime) = 0;
	void set_pos(float xx, float yy);
	float getposx();
	float getposy();
	float getrposx();
	float getrposy();
	Position* getpos();
	virtual Position* new_pos(double dtime) = 0;
	virtual char type() = 0; // return char to tell what type of entity this is
	
};

