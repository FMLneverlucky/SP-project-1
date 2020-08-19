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

void Wall::update_pos(float dtime)
{

};

Position* Wall::new_pos(float dtime)
{
	//no codes
	Position p;
	return &p;
}