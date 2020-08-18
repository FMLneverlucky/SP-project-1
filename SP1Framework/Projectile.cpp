#include "Projectile.h"

Projectile::Projectile()
{
	x = 40;
	y = 0;
	m_x = 0;
	m_y = 0;
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

void Projectile::set_mpos(int mx, int my)
{
	m_x = mx;
	m_y = my;
}

void Projectile::update_particle()
{
	if (dir == left)
		this->x = x - 1;

	else if (dir == right)
		this->x += 1;

	else if (dir == down)
		this->y += 1;

	else
		this->y -= 1;
}

int Projectile::get_px(void)
{
	return x;
}

int Projectile::get_py(void)
{
	return y;
}

int Projectile::get_mx(void)
{
	return m_x;
}

int Projectile::get_my(void)
{
	return m_y;
}

void Projectile::direction()
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