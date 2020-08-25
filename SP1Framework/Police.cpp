#include "Police.h"

Police::Police()
{

}

Police::Police(int cd) 
{
	freezetime = cd;
}

char Police::type()
{
	return 'B';
}