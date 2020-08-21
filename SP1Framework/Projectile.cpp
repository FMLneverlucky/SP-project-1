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

void Projectile::set_newpos()
{
	x = newpos.get_x();
	y = newpos.get_y();
}

void Projectile::update_particle(float delta)
{
	if (dir == left)
		this->x -= (spacecount * delta);

	else if (dir == top_left)
	{
		this->x -= (spacecount * delta);
		this->y -= (spacecount * delta);
	}

	else if (dir == top_right)
	{
		this->x += (spacecount * delta);
		this->y -= (spacecount * delta);
	}

	else if (dir == right)
		this->x += (spacecount * delta);

	else if (dir == down)
		this->y -= (spacecount * delta);

	else if (dir == btm_left)
	{
		this->x -= (spacecount * delta);
		this->y += (spacecount * delta);
	}
	else if (dir == btm_right)
	{
		this->x += (spacecount * delta);
		this->y += (spacecount * delta);
	}

	else
		this->y += (spacecount * delta);
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
	newpos.set_x(x);
	newpos.set_y(y);

	if ((m_x < x) && ((x - m_x) > (y - m_y)) && ((x - m_x) > (m_y - y)))
	{
		dir = left;
		spacecount = 20;
	}

	else if ((m_x < x) && (m_y < y))
	{
		dir = top_left;
		spacecount = 15;
	}

	else if ((m_x > x) && ((m_x - x) > (y - m_y)) && ((m_x - x) > (m_y - y)))
	{
		dir = right;
		newpos.set_x(x + 1);
		spacecount = 20;
	}

	else if ((m_x > x) && (m_y < y))
	{
		dir = top_right;
		newpos.set_x(x + 1);
		spacecount = 15;
	}

	else if ((m_x > x) && (m_y > y))
	{
		dir = btm_right;
		spacecount = 20;
	}

	else if ((m_x < x) && (m_y > y))
	{
		dir = btm_left;
		newpos.set_y(y + 1);
		spacecount = 15;
	}

	else if (m_y < y)
	{
		dir = down;
		newpos.set_y(y + 1);
		spacecount = 20;
	}
	else
	{
		dir = up;
		spacecount = 20;
	}

}

Position* Projectile::getpos()
{
	pos.set_x(x);
	pos.set_y(y);
	return &pos;
}

