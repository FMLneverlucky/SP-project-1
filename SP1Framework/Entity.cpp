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

void Entity::set_pos()
{
	switch (direction)
	{
	case UP:
		if (pos.get_y() > 0)
		{
			pos.set_y(pos.get_y() - 0.1);
		}
		break;
	case DOWN:
		if (pos.get_y() < 24)
		{
			pos.set_y(pos.get_y() + 0.1);
		}
		break;
	case LEFT:
		if (pos.get_x() > 0)
		{
			pos.set_x(pos.get_x() - 0.2);
		}
		break;
	case RIGHT:
		if (pos.get_x() < 79)
		{
			pos.set_x(pos.get_x() + 0.2);
		}
		break;
	case NOT:
		break;
	}
}

void Entity::set_pos(float speed)
{
	switch (direction)
	{
	case UP:
		if (pos.get_y() > 0)
		{
			pos.set_y(pos.get_y() - (0.1 * speed));
		}
		break;
	case DOWN:
		if (pos.get_y() < 24)
		{
			pos.set_y(pos.get_y() + (0.1 * speed));
		}
		break;
	case LEFT:
		if (pos.get_x() > 0)
		{
			pos.set_x(pos.get_x() - (0.2 * speed));
		}
		break;
	case RIGHT:
		if (pos.get_x() < 79)
		{
			pos.set_x(pos.get_x() + (0.2 * speed));
		}
		break;
	case NOT:
		break;
	}
}

void Entity::set_pos(float xx, float yy)
{
	pos.set_x(xx);
	pos.set_y(yy);
}

