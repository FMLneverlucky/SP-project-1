#pragma once
#include "Position.h"

enum face
{
	none,
	up,
	down,
	top_left,
	left,
	top_right,
	right,
	btm_right,
	btm_left
};

class Projectile
{
	double x, y;
	face dir;
	int spacecount;
	int p_cooldown;
	Position pos;
	Position newpos;

public:
	Projectile();
	~Projectile();

	void set_ppos(double px, double py);
	double get_px(void);
	double get_py(void);

	void set_newpos();
	void direction(int m_x, int m_y);
	void update_particle(double delta);

	void set_spacecount(int fc);
	int get_spacecount();

	int get_pcooldown();
	void set_pcooldown(int pc);
	
	Position* getpos();
};

