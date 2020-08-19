#include "Wall.h"

Wall::Wall()
{
	
};

Wall::~Wall()
{

};

char Wall::type()
{
	switch (wall)
	{
	case 1:
		return 'R';
		break;
	case 2:
		return 'O';
		break;
	case 3:
		return 'Y';
		break;
	case 4:
		return 'G';
		break;
	case 5:
		return 'L';
		break;
	case 6:
		return 'B';
		break;
	case 7:
		return 'P';
		break;
	}
};

void Wall::update_pos(float dtime)
{

};

Position* Wall::new_pos(float dtime)
{
	Position p;
	return &p;
}