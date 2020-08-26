#include "Zone.h"

void Zone::setpos(int x, int y)
{
	pos[4].set_pos(x, y); //sets pos of 4 (mid of zone)

    for (int i = -1; i < 2; i++)
    {
        pos[1 + i].set_pos(pos[4].get_x() + i, pos[4].get_y() - 1); //sets pos of 0 1 2
        pos[7 + i].set_pos(pos[4].get_x() + i, pos[4].get_y() + 1); //sets pos of 6 7 8
    }

    pos[3].set_pos(pos[4].get_x() - 1, pos[4].get_y()); //sets pos of 3
    pos[5].set_pos(pos[4].get_x() + 1, pos[4].get_y()); //sets pos of 5

    //0 1 2
    //3 4 5
    //6 7 8
}

Position* Zone::getpos(int index)
{
    return &pos[index];
}
