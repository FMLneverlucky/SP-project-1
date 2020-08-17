#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

int Entity::getposx()
{
	return pos.get_x();
}

int Entity::getposy()
{
	return pos.get_y();
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

void Entity::set_pos()
{
	switch (direction)
	{
	case UP:
		pos.set_y(pos.get_y() + 1);
		break;
	case DOWN:
		pos.set_y(pos.get_y() - 1);
		break;
	case LEFT:
		pos.set_x(pos.get_x() - 1);
		break;
	case RIGHT:
		pos.set_x(pos.get_x() + 1);
		break;
	default:
		break;
	}
}

void Entity::set_pos(int xx, int yy)
{
	pos.set_x(xx);
	pos.set_y(yy);
}
