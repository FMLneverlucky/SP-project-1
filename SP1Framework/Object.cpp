#include"Object.h"
Object::Object(int sizeX = 1, int sizeY = 1)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	scaleX = 1;
	scaleY = 1;
	//init pos and pivot
}
Object::Object(int sizeX = 1, int sizeY = 1, Position pos)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	scaleX = 1;
	scaleY = 1;
	this->pos = pos;
	//init pivot
}
Object::Object(int sizeX = 1, int sizeY = 1,Position pos, Position pivot)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	scaleX = 1;
	scaleY = 1;
	this->pos = pos;
	this->pivot = pivot;
}
Object::~Object()
{
	//Nothing.
}
void Object::size(int x, int y)
{
	sizeX = x;
	sizeY = y;
}
void Object::scale(int x, int y)
{
	scaleX = x;
	scaleY = y;
	//change size according to pivot point
}
void Object::translate(int x, int y)
{
	//change pos
}
int Object::length()
{
	return sizeX * scaleX;
}
int Object::height()
{
	return sizeY * scaleY;
}
Position* Object::position()
{
	return pos*;
}
