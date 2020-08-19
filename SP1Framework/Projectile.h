#pragma once

enum face
{
	none,
	up,
	down,
	left,
	right
};

class Projectile
{
	int x, y;
	face dir;

public:
	Projectile();
	~Projectile();
	void set_ppos(int px, int py);
	void update_particle();
	int get_px(void);
	int get_py(void);
	void direction(int m_x, int m_y);
};

