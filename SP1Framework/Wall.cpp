#include "Wall.h"

Wall::Wall()
{

};

Wall::~Wall()
{

};

char Wall::type()
{
	return 'W';
};

void Wall::update_pos()
{

};

Position* Wall::new_pos()
{
	//no codes
	Position p;
	return &p;
}