#include "NPC.h"

NPC::NPC() :angry(false), dead(false), counter(0), speed(0.1), damage(1)
{
	
}

NPC::~NPC()
{
	//no codes yet
}

bool NPC::isHostile()
{
	return angry;
}

void NPC::anger()
{
	angry = true;
}

void NPC::set_count(int a)
{
	counter = a;
}

int NPC::get_count()
{
	return counter;
}

char NPC::type()
{
	return 'C'; // C for civilian. do diff char for diff derived class
}

float NPC::get_speed()
{
	return speed;
}

void NPC::set_speed(float spd)
{
	speed = spd;
}

Position* NPC::new_pos()
{
	
	tempp.set_x(pos.get_x());
	tempp.set_y(pos.get_y());


	switch (direction)
	{
	case UP:
		if (pos.get_y() > 0)
		{
			tempp.set_y(pos.get_y() - (0.1 * speed));
		}
		break;
	case DOWN:
		if (pos.get_y() < 24)
		{
			tempp.set_y(pos.get_y() + (0.1 * speed));
		}
		break;
	case LEFT:
		if (pos.get_x() > 0)
		{
			tempp.set_x(pos.get_x() - (0.2 * speed));
		}
		break;
	case RIGHT:
		if (pos.get_x() < 79)
		{
			tempp.set_x(pos.get_x() + (0.2 * speed));
		}
		break;
	case NOT:
		break;
	}

	return &tempp;
}

void NPC::update_pos()
{
	pos.set_x(new_pos()->get_x());
	pos.set_y(new_pos()->get_y());
}