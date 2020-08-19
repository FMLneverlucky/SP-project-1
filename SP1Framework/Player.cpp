#include "Player.h"
Player::Player() : HP(5)
{

}
Player::~Player()
{
	//Nothing.
}
char Player::type()
{
	return 'P';
}