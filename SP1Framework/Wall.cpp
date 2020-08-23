#include "Wall.h"

//position things




Wall::Wall()
{
	
};

Wall::~Wall()
{

};

char Wall::type()
{																					//this function is for checking what type of entity it is (since entity pointer cannot tell)
	return 'W';																		//this is the virtual function from entity
};																					//create another gettype or smt if u meant to use this for diff types of wall; as in 3x1 2x2 and so on

int Wall::setType()
{
	int x = rand() % 8;
	int nextWall;
	switch (x)
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
	default:
		nextWall = d;

		return nextWall;
	}
}

void Wall::typeI()
{

}

void Wall::typeJ()
{

}

void Wall::typeL()
{

}

void Wall::typeO()
{

}

void Wall::typeS()
{

}

void Wall::typeT()
{

}

void Wall::typeZ()
{

}

void Wall::typed()
{

}

void Wall::update_pos(float dtime)
{

};

Position* Wall::new_pos(float dtime)
{
	Position p;
	return &p;
}