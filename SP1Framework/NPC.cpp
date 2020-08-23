#include "NPC.h"
int NPC::noHostile = 0;

NPC::NPC(int cd) :angry(false), dead(false), counter(0), speed(0.1), damage(1), freezetime(0), cooldown(cd)
{
	//normal civilians use this
}

NPC::NPC() :angry(false), dead(false), counter(0), speed(0.1), damage(5), freezetime(0), cooldown(1)
{
	//police use this
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
	counter = 0;
	noHostile++;
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

Position* NPC::new_pos(double dtime)
{
	
	tempp.set_x(pos.get_x());
	tempp.set_y(pos.get_y());


	switch (direction)
	{
	case UP:
		if (static_cast<int>(pos.get_y()) > 1)
		{
			tempp.set_y(pos.get_y() - (10 * speed * dtime));
		}
		break;
	case DOWN:
		if (pos.get_y() < 24)
		{
			tempp.set_y(pos.get_y() + (10 * speed * dtime));
		}
		break;
	case LEFT:
		if (pos.get_x() > 0)
		{
			tempp.set_x(pos.get_x() - (20 * speed * dtime));
		}
		break;
	case RIGHT:
		if (pos.get_x() < 79)
		{
			tempp.set_x(pos.get_x() + (20 * speed * dtime));
		}
		break;
	case NOT:
		break;
	}

	return &tempp;
}

int NPC::get_damage()
{
	return damage;
}

int NPC::get_ftime()
{
	return freezetime;
}

void NPC::update_pos(double dtime)
{
	pos.set_x(new_pos(dtime)->get_x());
	pos.set_y(new_pos(dtime)->get_y());
}

void NPC::cooldownstart()
{
	freezetime = cooldown;
}

void NPC::cooldownend()
{
	freezetime = 0;
}

int NPC::getnoHostile()
{
	return noHostile;
}

void NPC::resetnoHostile()
{
	noHostile = 0;
}