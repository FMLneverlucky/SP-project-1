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

Position* Player::new_pos(float dtime)
{
	tempp.set_x(this->pos.get_x());
	tempp.set_y(this->pos.get_y());

	switch (direction)
	{
	case UP:
		if (pos.get_y() > 0)
		{
			tempp.set_y(tempp.get_y() - (1 * dtime));
		}
		break;
	case DOWN:
		if (pos.get_y() < 24)
		{
			tempp.set_y(tempp.get_y() + (1 * dtime));
		}
		break;
	case LEFT:
		if (pos.get_x() > 0)
		{
			tempp.set_x(tempp.get_x() - (2 * dtime));
		}
		break;
	case RIGHT:
		if (pos.get_x() < 79)
		{
			tempp.set_x(tempp.get_x() + (2 * dtime));
		}
		break;
	case NOT:
		break;
	}

	return &tempp;
}

void Player::update_pos(float dtime)
{
	pos.set_x(new_pos(dtime)->get_x());
	pos.set_y(new_pos(dtime)->get_y());
}