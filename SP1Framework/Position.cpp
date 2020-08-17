#include "Position.h"
Position::Position(int x, int y)
{
	this->x = x;
	this->y = y;
}

Position::Position():x(0), y(0)
{

}

float Position::get_x()
{
	return x;
}

float Position::get_y()
{
	return y;
}

void Position::set_x(float newx)
{
	x = newx;
}

void Position::set_y(float newy)
{
	y = newy;
}