#include "Player.h"
Player::Player() : HP(5), cooldown(0)
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

Position* Player::new_pos(double dtime)
{
	tempp.set_x(this->pos.get_x());
	tempp.set_y(this->pos.get_y());

	switch (direction)
	{
	case UP:
		if (pos.get_y() > 0)
		{
			tempp.set_y(tempp.get_y() - (10 * dtime));
		}
		break;
	case DOWN:
		if (pos.get_y() < 24)
		{
			tempp.set_y(tempp.get_y() + (10 * dtime));
		}
		break;
	case LEFT:
		if (pos.get_x() > 0)
		{
			tempp.set_x(tempp.get_x() - (20 * dtime));
		}
		break;
	case RIGHT:
		if (pos.get_x() < 79)
		{
			tempp.set_x(tempp.get_x() + (20 * dtime));
		}
		break;
	case NOT:
		break;
	}

	return &tempp;
}

void Player::update_pos(double dtime)
{
	pos.set_x(new_pos(dtime)->get_x());
	pos.set_y(new_pos(dtime)->get_y());
}

int Player::get_HP()
{
	return HP;
}

void Player::resetHP()
{
	HP = 5;
}

void Player::loseHP(int ack)
{
	HP -= ack;
}

void Player::set_cooldown(int a)
{
	cooldown = a;
}

int Player::get_cooldown()
{
	return cooldown;
}