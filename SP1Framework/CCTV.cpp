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
			direction = RIGHT;
			break;
		case DOWN:
			direction = LEFT;
			break;
		case LEFT:
			direction = UP;
			break;
		case RIGHT:
			direction = DOWN;
			break;
		
		}
	}
	else
	{
		switch (direction)
		{
		case UP:
			direction = LEFT;
			break;
		case DOWN:
			direction = RIGHT;
			break;
		case LEFT:
			direction = DOWN;
			break;
		case RIGHT:
			direction = UP;
			break;
		}
	}

	//updates new position
	switch (direction)
	{
	case UP:
		radar[0].set_pos(pos.get_x(), pos.get_y() - 1);
		for (int r = -1; r < 2; r++)
		{
			radar[2 + r].set_pos(pos.get_x() + r, pos.get_y() - 2);
		}
		for (int r = -2; r < 3; r++)
		{
			radar[6 + r].set_pos(pos.get_x() + r, pos.get_y() - 3);
		}
		for (int r = -3; r < 4; r++)
		{
			radar[12 + r].set_pos(pos.get_x() + r, pos.get_y() - 4);
		}
		for (int r = -4; r < 5; r++)
		{
			radar[20 + r].set_pos(pos.get_x() + r, pos.get_y() - 5);
		}
		break;
	case DOWN:
		radar[0].set_pos(pos.get_x(), pos.get_y() + 1);
		for (int r = -1; r < 2; r++)
		{
			radar[2 + r].set_pos(pos.get_x() + r, pos.get_y() + 2);
		}
		for (int r = -2; r < 3; r++)
		{
			radar[6 + r].set_pos(pos.get_x() + r, pos.get_y() + 3);
		}
		for (int r = -3; r < 4; r++)
		{
			radar[12 + r].set_pos(pos.get_x() + r, pos.get_y() + 4);
		}
		for (int r = -4; r < 5; r++)
		{
			radar[20 + r].set_pos(pos.get_x() + r, pos.get_y() + 5);
		}
		break;
	case LEFT:
		radar[0].set_pos(pos.get_x() - 1, pos.get_y());
		for (int r = -1; r < 2; r++)
		{
			radar[2 + r].set_pos(pos.get_x() - 2, pos.get_y() + r);
		}
		for (int r = -2; r < 3; r++)
		{
			radar[6 + r].set_pos(pos.get_x() - 3, pos.get_y() + r);
		}
		for (int r = -3; r < 4; r++)
		{
			radar[12 + r].set_pos(pos.get_x() - 4, pos.get_y() + r);
		}
		for (int r = -4; r < 5; r++)
		{
			radar[20 + r].set_pos(pos.get_x() - 5, pos.get_y() + r);
		}
		break;
	case RIGHT:
		radar[0].set_pos(pos.get_x() + 1, pos.get_y());
		for (int r = -1; r < 2; r++)
		{
			radar[2 + r].set_pos(pos.get_x() + 2, pos.get_y() + r);
		}
		for (int r = -2; r < 3; r++)
		{
			radar[6 + r].set_pos(pos.get_x() + 3, pos.get_y() + r);
		}
		for (int r = -3; r < 4; r++)
		{
			radar[12 + r].set_pos(pos.get_x() +4, pos.get_y() + r);
		}
		for (int r = -4; r < 5; r++)
		{
			radar[20 + r].set_pos(pos.get_x() + 5, pos.get_y() + r);
		}
		break;
	}

}

Position* CCTV::getRadarPos(int index)
{
	return &radar[index];
}

bool CCTV::isSameRow(int index, int index2)
{
	switch (index)
	{
	case 0:
		return true;
	case 1:
		if (index2 == 4 || index2 == 9 || index2 == 16)
		{
			return true;
		}
		break;
	case 2:
		if (index2 == 5 || index2 == 6 || index2 == 7 || (index2 <= 14 && index2 >= 10) || (index2 <= 23 && index2 >= 17))
		{
			return true;
		}
		break;
	case 3:
		if (index2 == 8 || index2 == 15 || index2 == 24)
		{
			return true;
		}
		break;
	case 4:
		if (index2 == 9 || index2 == 16)
		{
			return true;
		}
		break;
	case 5:
		if (index2 == 10 || index2 == 11 || index2 == 17 || index2 == 18 || index2 == 19)
		{
			return true;
		}
		break;
	case 6:
		if (index2 == 12 || index2 == 20)
		{
			return true;
		}
		break;
	case 7:
		if (index2 == 13 || index2 == 14 || index2 == 21 || index2 == 22 || index2 == 23)
		{
			return true;
		}
		break;
	case 8:
		if (index2 == 15 || index2 == 24)
		{
			return true;
		}
		break;
	case 9:
		if (index2 == 16 || index2 == 17)
		{
			return true;
		}
		break;
	case 10:
		if (index2 == 17 || index2 == 18)
		{
			return true;
		}
		break;
	case 11:
		if (index2 == 18 || index2 == 19)
		{
			return true;
		}
		break;
	case 12:
		if (index2 == 20 || index2 == 21)
		{
			return true;
		}
		break;
	case 13:
		if (index2 == 21 || index2 == 22)
		{
			return true;
		}
		break;
	case 14:
		if (index2 == 22 || index2 == 23)
		{
			return true;
		}
		break;
	case 15:
		if (index2 == 23 || index2 == 24)
		{
			return true;
		}
		break;
	
	}
	return false;
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

void CCTV::disable(int index)
{
	radar[index].set_pos(pos.get_x(), pos.get_y());
	for (int i = index + 1; i < 25; i++)
	{
		if (isSameRow(index, i))
		{
			radar[i].set_pos(pos.get_x(), pos.get_y());
		}
	}
}