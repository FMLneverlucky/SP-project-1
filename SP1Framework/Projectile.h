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
	float x, y;
	face dir;
	int spacecount;
	Position pos;
	Position newpos;

public:
	Projectile();
	~Projectile();
	void set_ppos(int px, int py);
	void set_newpos();
	void update_particle(float delta);
	int get_px(void);
	int get_py(void);
	void set_spacecount(int fc);
	int get_spacecount();
	
	void direction(int m_x, int m_y);
	Position* getpos();
};

