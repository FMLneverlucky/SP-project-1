#include "Player.h"
Player::Player() : HP(5)
{

}
Player::~Player()
{
	//Nothing.
}
char Player::type()
{
	return 'P';
}

void Player::update_pos()
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