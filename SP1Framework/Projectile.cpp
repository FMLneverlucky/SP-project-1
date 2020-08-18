#include "Projectile.h"

Projectile::Projectile()
{
	x = 40;
	y = 0;
	dir = none;
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
		this->x = x - 1;

	else if (dir == right)
		this->x += 1;

	else if (dir == down)
		this->y -= 1;

	else
		this->y += 1;
}

int Projectile::get_px(void)
{
	return x;
}

int Projectile::get_py(void)
{
	return y;
}

void Projectile::direction(int m_x, int m_y)
{
	if (m_x < x)
		dir = left;
	else if (m_x > x)
		dir = right;
	else if (m_y < y)
		dir = down;
	else
		dir = up;
}