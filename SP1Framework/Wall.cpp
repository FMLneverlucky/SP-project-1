#include "Wall.h"

Wall::Wall()
{
	
};

Wall::~Wall()
{

};

char Wall::type()
{
	//this function is for checking what type of entity it is (since entity pointer cannot tell)
	//this is the virtual function from entity
	//create another gettype or smt if u meant to use this for diff types of wall; as in 3x1 2x2 and so on

	/*switch (*expression*)
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
	}*/

	return 'W';
};

void Wall::update_pos(float dtime)
{

};

Position* Wall::new_pos(float dtime)
{
	Position p;
	return &p;
}