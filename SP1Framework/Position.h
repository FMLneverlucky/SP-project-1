#pragma once
class Position
{
private:
	int x;
	int y;

public:
	Position();
	Position(int, int);
	~Position();
	void setX(int);
	void setY(int);
	int get_x(void);
	int get_y(void);
};

