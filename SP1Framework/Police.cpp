#include "Police.h"

Police::Police()
{

}

Police::Police(int cd) 
{
	cooldown = cd;
}

char Police::type()
{
	return 'B';
}