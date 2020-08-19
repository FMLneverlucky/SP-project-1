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
