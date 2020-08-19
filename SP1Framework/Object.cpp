#include"Object.h"
Object::Object(int sizeX = 1, int sizeY = 1)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	scaleX = 1;
	scaleY = 1;
	pos.set_x(1);
	pos.set_y(1);
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
void Object::scale(int x, int y)
{
	sizeX *= x;
	sizeY *= y;
	pos.set_x(pos.get_x() + (pos.get_x() - pivot.get_x()) * x);
	pos.set_y(pos.get_y() + (pos.get_y() - pivot.get_y()) * y);
}
void Object::translate(float x, float y)
{
	pos.set_x(pos.get_x() + x);
	pos.set_y(pos.get_y() + y);
}
void Object::setPivot(int x, int y)
{
	pivot.set_x(x);
	pivot.set_y(y);
}
int Object::length()
{
	return sizeX;
}
int Object::height()
{
	return sizeY;
}
Position* Object::position()
{
	return &pos;
}
Position* Object::referencePosition()
{
	referencePos.set_x(pos.get_x() - (this->sizeX / 2.0));
	referencePos.set_y(pos.get_y() - (this->sizeY / 2.0));
	return &referencePos;
}