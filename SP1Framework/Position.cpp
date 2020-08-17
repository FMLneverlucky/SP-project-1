#include "Position.h"

Position::Position():x(0), y(0)
{

}

int Position::get_x()
{
	return x;
}

int Position::get_y()
{
	return y;
}

void Position::set_x(int newx)
{
	x = newx;
}

void Position::set_y(int newy)
{
	y = newy;
}