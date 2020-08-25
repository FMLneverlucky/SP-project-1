#include "PowerUp.h"


PowerUp::PowerUp()
{
	detime = 0;
}

PowerUp::~PowerUp()
{

}

void PowerUp::set_xcoord(float a)
{
	UPpos.set_x(a);
}

void PowerUp::set_ycoord(float b)
{
	UPpos.set_y(b);
}

float PowerUp::get_xcoord()
{
	return UPpos.get_x();
}

float PowerUp::get_ycoord()
{
	return UPpos.get_y();
}

void PowerUp::set_detime(int d)
{
	detime = d;
}

int PowerUp::get_detime()
{
	return detime;
}

Position* PowerUp::get_pos()
{
	return &UPpos;
}
