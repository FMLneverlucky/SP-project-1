#include "Map.h"

int main(void)
{
	for (int row = 0; row < 80; row++)
		for (int col = 0; col < 24; col++)
			bool maparray[80][24] = { false };
};

bool isSpaceEmpty(float x, float y)
{
	if (bool maparray[x - 1][y - 1] == false)
		return true;
};