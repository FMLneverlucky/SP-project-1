#pragma once
#include"Position.h"
class Object
{
	int sizeX;
	int sizeY;
	Position pos;
	Position pivot;
	int scaleX;
	int scaleY;
public:
	Object();
	Object(int, int);
	Object(int, int, Position);
	Object(int, int, Position, Position);
	~Object();
	void size(int, int);
	void scale(int, int);
	void translate(int, int);
	int length();
	int height();
	Position* position();
};
