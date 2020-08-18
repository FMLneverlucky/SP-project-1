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
	int x, y, m_x, m_y;
	face dir;

public:
	Projectile();
	~Projectile();
	void set_ppos(int px, int py);
	void set_mpos(int mx, int my);
	void update_particle();
	int get_px(void);
	int get_py(void);
	int get_mx(void);
	int get_my(void);
	void direction();
};

