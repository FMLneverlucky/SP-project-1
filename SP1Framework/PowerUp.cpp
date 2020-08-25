#include "PowerUp.h"


PowerUp::PowerUp()
{
	
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

Position* PowerUp::get_pos()
{
	return &UPpos;
}
