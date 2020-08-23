#include "Projectile.h"

Projectile::Projectile()
{
	x = 40;
	y = 0;
	dir = none;
	spacecount = 0;
	p_cooldown = 0;
}

Projectile::~Projectile()
{
}

double Projectile::get_px(void)
{
	return x;
}

double Projectile::get_py(void)
{
	return y;
}

void Projectile::set_ppos(double px, double py)
{
	x = px;
	y = py;
}

void Projectile::set_newpos()
{
	x = newpos.get_x();
	y = newpos.get_y();
}

void Projectile::direction(int m_x, int m_y)
{
	newpos.set_x(x);
	newpos.set_y(y);

	if ((m_x < 40) && ((40 - m_x) > (12 - m_y)) && ((40 - m_x) > (m_y - 12)))
	{
		dir = left;
		spacecount = 20;
	}

	else if ((m_x < 40) && (m_y < 12))
	{
		dir = top_left;
		spacecount = 15;
	}

	else if ((m_x > 40) && ((m_x - 40) > (12 - m_y)) && ((m_x - 12) > (m_y - 40)))
	{
		dir = right;
		newpos.set_x(x + 1);
		spacecount = 20;
	}

	else if ((m_x > 40) && (m_y < 12))
	{
		dir = top_right;
		newpos.set_x(x + 1);
		spacecount = 15;
	}

	else if ((m_x > 40) && (m_y > 12))
	{
		dir = btm_right;
		spacecount = 20;
	}

	else if ((m_x < 40) && (m_y > 12))
	{
		dir = btm_left;
		newpos.set_y(y + 1);
		spacecount = 15;
	}

	else if (m_y < 12)
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

void Projectile::update_particle(double delta)
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
		this->y -= (spacecount * delta);//
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

void Projectile::set_spacecount(int fc)
{
	spacecount = fc;
}

int Projectile::get_spacecount()
{
	return spacecount;
}

int Projectile::get_pcooldown()
{
	return p_cooldown;
}
void Projectile::set_pcooldown(int pc)
{
	p_cooldown = pc;
}

Position* Projectile::getpos()
{
	pos.set_x(x);
	pos.set_y(y);
	return &pos;
}
