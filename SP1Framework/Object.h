#pragma once
#include"Position.h"
class Object
{// this can only be a rectangle or square 
	int sizeX;
	int sizeY;
	Position pos;// the center of the object
	Position pivot;// pivot point for scaling
	Position referencePos;// top left position
	int scaleX;
	int scaleY;
public:
	Object(int, int);
	Object(int, int, Position);
	Object(int, int, Position, Position);
	~Object();
	//set functions
	void scale(int, int);// change size according to pivot point, multiplies the lengths
	void translate(float, float);// moves the object
	void setPivot(int, int);
	//get functions
	int length();
	int height();
	Position* position();// get center of object
	Position* referencePosition();// get top left position of object
	char type();// know what type of object this is
};