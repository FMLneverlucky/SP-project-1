#include"Object.h"
Object::Object(int sizeX = 1, int sizeY = 1)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
}
Object::Object(int sizeX, int sizeY, Position pos)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->pos = pos;
}
Object::Object(int sizeX, int sizeY, Position pos, Position pivot)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->pos = pos;
	this->pivot = pivot;
}
Object::~Object()
{
	//Nothing.
}
void Object::move(float x, float y)
{
	pivot.set_x(pivot.get_x() + (x - pos.get_x()));
	pivot.set_y(pivot.get_y() + (y - pos.get_y()));
	pos.set_x(x);
	pos.set_y(y);
}
void Object::resize(int x, int y) 
{
	referencePosition();
	sizeX = x;
	sizeY = y;
	pos.set_x(referencePos.get_x() - x / 2.0);
	pos.set_y(referencePos.get_y() - y / 2.0);
}
void Object::scale(float x, float y)
{
	sizeX *= x;
	sizeY *= y;
	pos.set_x(pos.get_x() + (pos.get_x() - pivot.get_x()) * (x - 1));
	pos.set_y(pos.get_y() + (pos.get_y() - pivot.get_y()) * (y - 1));
}
void Object::translate(float x, float y)
{
	pos.set_x(pos.get_x() + x);
	pos.set_y(pos.get_y() + y);
	pivot.set_x(pivot.get_x() + x);
	pivot.set_y(pivot.get_y() + y);
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
	referencePos.set_x(pos.get_x() - (sizeX / 2.0));
	referencePos.set_y(pos.get_y() - (sizeY / 2.0));
	return &referencePos;
}