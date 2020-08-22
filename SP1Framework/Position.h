#pragma once
class Position
{
private:
	float x;
	float y;

public:
	Position();
	Position(float x, float y);
	~Position();
	float get_x(void);
	float get_y(void);
	void set_x(float newx);
	void set_y(float newy);
	void set_pos(float x, float y);
};

