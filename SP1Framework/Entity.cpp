#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

float Entity::getposx()
{
	return pos.get_x();
}

float Entity::getposy()
{
	return pos.get_y();
}
Position* Entity::getpos()
{
	return &pos;
}

void Entity::set_direction(int a)
{
	switch (a)
	{
	case 1:
		direction = UP;
		break;
	case 2:
		direction = DOWN;
		break;
	case 3:
		direction = LEFT;
		break;
	case 4:
		direction = RIGHT;
		break;
	default:
		direction = NOT;
	}
}





void Entity::set_pos(float xx, float yy)
{
	pos.set_x(xx);
	pos.set_y(yy);
}



