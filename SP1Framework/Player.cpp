#include "Player.h"
Player::Player() : maxHP(5), flash(false), lethal(false), lethality_duration(0), kills(0)
{
	HP = maxHP;
}

Player::~Player()
{
	//Nothing.
}

char Player::type()
{
	return 'P';
}

void Player::set_flash(bool what)
{
	flash = what;
}

bool Player::get_flash()
{
	return flash;
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

int Player::get_maxHP()
{
	return maxHP;
}

void Player::resetHP()
{
	HP = 5;
}

void Player::loseHP(int ack)
{
	HP -= ack;
}

int Player::get_lethalstatus()
{
	return lethal; // 0 = false, 1 = true
}

void Player::set_lethal()
{
	lethal = true;
	lethality_duration = 900;
}

void Player::update_ld()
{
	lethality_duration -= 1;
	if (lethality_duration == 0)
		lethal = false;
}

void Player::resetlethality()
{
	lethal = false;
	lethality_duration = 0;
}

void Player::prevDamaged(char e)
{
	prevDamage = e;
}

char Player::getPrevDamaged()
{
	return prevDamage;
}

void Player::addKills(int no)
{
	kills += no;
}

int Player::getKills()
{
	return kills;
}

void Player::resetKills()
{
	kills = 0;
}