#include "Position.h"
Position::Position()
{
	x = 0;
	y = 0;
}
Position::Position(int x, int y)
{
	this->x = x;
	this->y = y;
}
Position::~Position()
{
	//Nothing.
}
void Position::setX(int x)
{
	this->x = x;
}
void Position::setY(int y)
{
	this->y = y;
}

int Position::get_x()
{
	return x;
}

int Position::get_y()
{
	return y;
}