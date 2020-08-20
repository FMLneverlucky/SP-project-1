#include "Projectile.h"

Projectile::Projectile()
{
	x = 40;
	y = 0;
	dir = none;
	spacecount = 0;
}

Projectile::~Projectile()
{
}

void Projectile::set_ppos(int px, int py)
{
	x = px;
	y = py;
}

void Projectile::update_particle()
{
	if (dir == left)
		this->x -= 0.2;

	else if (dir == right)
		this->x += 0.2;

	else if (dir == down)
		this->y -= 0.2;

	else
		this->y += 0.2;
}

int Projectile::get_px(void)
{
	return x;
}

int Projectile::get_py(void)
{
	return y;
}

void Projectile::set_spacecount(int fc)
{
	spacecount = fc;
}

int Projectile::get_spacecount()
{
	return spacecount;
}

void Projectile::direction(int m_x, int m_y)
{
	if ((m_x < x) && ((x - m_x) > (y - m_y)) && ((x - m_x) > (m_y - y)))
	{
		dir = left;
		spacecount = 20;
	}

	else if ((m_x > x) && ((m_x - x) > (y - m_y)) && ((m_x - x) > (m_y - y)))
	{
		dir = right;
		spacecount = 20;
	}
	else if (m_y < y)
	{
		dir = down;
		spacecount = 10;
	}
	else
	{
		dir = up;
		spacecount = 10;
	}
}

Position* Projectile::getpos()
{
	pos.set_x(x);
	pos.set_y(y);
	return &pos;
}