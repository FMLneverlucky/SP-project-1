#include "CCTV.h"

CCTV::CCTV() : cw(true), rotateCD(0)
{
	//no codes yet
}

CCTV::CCTV(int dir, bool iscw) : rotateCD(0), cw(iscw)
{
	switch (dir)
	{
	case 1:
		direction = UP;
		break;
	case 2:
		direction = DOWN;
		break;
	case 3:
		direction = LEFT;
		break;
	case 4:
		direction = RIGHT;
		break;
	case 5:
		direction = TOPLEFT;
		break;
	case 6:
		direction = TOPRIGHT;
		break;
	case 7:
		direction = BOTTOMLEFT;
		break;
	case 8:
		direction = BOTTOMLEFT;
		break;
	}
}

void CCTV::setCD(double count)
{
	rotateCD = count;
}

int CCTV::getCD()
{
	return rotateCD;
}

void CCTV::update_cctv()
{
	//updates new direction
	if (cw)
	{
		switch (direction)
		{
		case UP:
			direction = TOPRIGHT;
			break;
		case DOWN:
			direction = BOTTOMLEFT;
			break;
		case LEFT:
			direction = TOPLEFT;
			break;
		case RIGHT:
			direction = BOTTOMRIGHT;
			break;
		case TOPLEFT:
			direction = UP;
			break;
		case TOPRIGHT:
			direction = RIGHT;
			break;
		case BOTTOMLEFT:
			direction = LEFT;
			break;
		case BOTTOMRIGHT:
			direction = DOWN;
			break;
		}
	}
	else
	{
		switch (direction)
		{
		case UP:
			direction = TOPLEFT;
			break;
		case DOWN:
			direction = BOTTOMRIGHT;
			break;
		case LEFT:
			direction = BOTTOMLEFT;
			break;
		case RIGHT:
			direction = TOPRIGHT;
			break;
		case TOPLEFT:
			direction = LEFT;
			break;
		case TOPRIGHT:
			direction = UP;
			break;
		case BOTTOMLEFT:
			direction = DOWN;
			break;
		case BOTTOMRIGHT:
			direction = RIGHT;
			break;
		}
	}

	//updates new position
	switch (direction)
	{
	case UP:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x(), pos.get_y() + r + 1);
		}
		break;
	case DOWN:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x(), pos.get_y() - r - 1);
		}
		break;
	case LEFT:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x() - r - 1, pos.get_y());
		}
		break;
	case RIGHT:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x() + r + 1, pos.get_y());
		}
		break;
	case TOPLEFT:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x() - r - 1, pos.get_y() + r + 1);
		}
		break;
	case TOPRIGHT:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x() + r + 1, pos.get_y() + r + 1);
		}
		break;
	case BOTTOMLEFT:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x() - r - 1, pos.get_y() - r - 1);
		}
		break;
	case BOTTOMRIGHT:
		for (int r = 0; r < 20; r++)
		{
			radar[r].set_pos(pos.get_x() + r + 1, pos.get_y() - r - 1);
		}
		break;
	}

}

Position* CCTV::getRadarPos(int index)
{
	return &radar[index];
}

char CCTV::type()
{
	return 'R';
}


void CCTV::update_pos(double dtime)
{
	//no codes
}
Position* CCTV::new_pos(double dtime)
{
	//no codes
	return &radar[0];
}