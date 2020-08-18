#include "Position.h"
Position::Position(float x, float y)
{
	this->x = x;
	this->y = y;
}

Position::Position():x(0), y(0)
{

}

Position::~Position()
{
	//Nothing.
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