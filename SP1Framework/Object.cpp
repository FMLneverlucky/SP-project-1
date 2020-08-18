#include"Object.h"
Object::Object()
{
	sizeX = 1;
	sizeY = 1;
	scaleX = 1;
	scaleY = 1;
	pos.set_x(0);
	pos.set_y(0);
	pivot.set_x(0);
	pivot.set_y(0);
}
Object::Object(int sizeX, int sizeY)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	scaleX = 1;
	scaleY = 1;
	pos.set_x(0);
	pos.set_y(0);
	pivot.set_x(0);
	pivot.set_y(0);
}
Object::Object(int sizeX, int sizeY, Position pos)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	scaleX = 1;
	scaleY = 1;
	this->pos = pos;
	pivot.set_x(0);
	pivot.set_y(0);
}
Object::Object(int sizeX, int sizeY, Position pos, Position pivot)
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
