#include "NPC.h"

NPC::NPC() :angry(false), dead(false)
{
	//no codes yet
}

NPC::~NPC()
{
	//no codes yet
}

bool NPC::isHostile()
{
	return angry;
}

