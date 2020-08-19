#include "Wall.h"

Wall::Wall()
{
	
};

Wall::~Wall()
{

};

char Wall::type()
{
	switch (*expression*)
	{
	case 1:
		return 'R';
	case 2:
		return 'O';
	case 3:
		return 'Y';
	case 4:
		return 'G';
	case 5:
		return 'L';
	case 6:
		return 'B';
	case 7:
		return 'P';
	}
};

void Wall::update_pos()
{

};

Position* Wall::new_pos()
{
	Position p;
	return &p;
}