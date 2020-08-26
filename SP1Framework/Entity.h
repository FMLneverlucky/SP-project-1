#pragma once
#include "Position.h"

enum direction //for Entity movement/anyth that involves direction
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
	direction direction;
	Position pos; //pos on map/console
	Position rpos; //pos relative to player
	Position tempp; //for temp use in code

public:
	Entity();
	~Entity();
	void set_direction(int a); //sets direction of entities
	void set_pos(float xx, float yy); //sets position based on passed in values
	void set_rpos(float x, float y); //sets relative position
	float getposx(); //returns x value of entity position
	float getposy(); //returns y value of entity position
	float getrposx(); //returns x value of entity's relative position
	float getrposy(); //returns y value of entity's relative position
	Position* getpos(); //returns pos

	virtual void update_pos(double dtime) = 0; //for moving, updates pos, pos + no. of sqr moved per frame
	virtual Position* new_pos(double dtime) = 0; //returns new pos which entity is about to move to/wld move to in that frame
	virtual char type() = 0; // return char to tell what type of entity this is
	
};

