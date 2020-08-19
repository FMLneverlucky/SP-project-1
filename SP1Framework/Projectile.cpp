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
													//data members store projectile position from char position
		this->x = x -= 0.5;                         //direction function determines projectile position from mouse position
	                                                //this means projectile will keep checking position from mouse and basically
	else if (dir == right)                          //update particle will take direction stored and update position of projectile
		this->x += 0.5;                             //the projectile shld only take the position of the mouse when space is pressed
	                                                //projectile shld take the position of mouse when space is pressed and travel
	else if (dir == down)                           //across the console and render out to be used again
		this->y -= 0.5;                             //how to only take mouse position when space is pressed
	                                                //put under space key event? since projectile is created when space is pressed
	else                                            //^ works except need limit traveling horizon dir to 2 n vertical dir to 4
		this->y += 0.5;                             // for int i = 0 and i is less than 4, loops 4 times, i will do increment, original x will add i IT FAST WTH
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

Position* Projectile::getpos()
{
	Position pos;
	pos.set_x(x);
	pos.set_y(y);
	return &pos;
}