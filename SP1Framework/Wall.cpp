#include <iostream>
#include <stdlib.h>
#include "Wall.h"
#include "Position.h"

Wall::Wall()
{
	
}

Wall::~Wall()
{

}

char Wall::type()
{																					//this function is for checking what type of entity it is (since entity pointer cannot tell)
	return 'W';																		//this is the virtual function from entity
}																				//create another gettype or smt if u meant to use this for diff types of wall; as in 3x1 2x2 and so on

void Wall::setPos(int x, int y)
{
    wallPos[0].set_x(x);                                                          //set x coordinate of variable, wallPos[0], as a number from 0 to 80
    wallPos[0].set_y(y);
    pos.set_x(x);
    pos.set_y(y);
    
    
}

void Wall::setPosForAll()
{
    int x = rand() % 30;
    switch (x)
    {
        //checks if next wall is 'I' piece and the different rotations of said piece
        case 1:
        {
            wallPos[1].set_x(wallPos[0].get_x());                                                        //setting x coord of second wall using previous pivot point as reference
            wallPos[1].set_y(wallPos[0].get_y() + 1);                                                    //setting y coord of second wall using previous pivot point as reference

            wallPos[2].set_x(wallPos[0].get_x());                                                        //setting x coord of third wall using previous pivot point as reference
            wallPos[2].set_y(wallPos[0].get_y() + 2);                                                    //setting y coord of third wall using previous pivot point as reference

            wallPos[3].set_x(wallPos[0].get_x());                                                        //setting x coord of fourth wall using previous pivot point as reference
            wallPos[3].set_y(wallPos[0].get_y() + 3);
            
            break;
        }

        case 2:
        {
            wallPos[1].set_x(wallPos[0].get_x() + 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() + 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() + 3);
            wallPos[3].set_y(wallPos[0].get_y());
            break;
        }

        case 3:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() - 1);

            wallPos[2].set_x(wallPos[0].get_x());
            wallPos[2].set_y(wallPos[0].get_y() - 2);

            wallPos[3].set_x(wallPos[0].get_x());
            wallPos[3].set_y(wallPos[0].get_y() - 3);
            break;
        }
        case 4:
        {
            wallPos[1].set_x(wallPos[0].get_x() - 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() - 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() - 3);
            wallPos[3].set_y(wallPos[0].get_y());
            break;
        }

        //checks if next wall is 'J' piece and the different rotations of said piece
        case 5:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() + 1);

            wallPos[2].set_x(wallPos[0].get_x());
            wallPos[2].set_y(wallPos[0].get_y() + 2);

            wallPos[3].set_x(wallPos[0].get_x() - 1);
            wallPos[3].set_y(wallPos[0].get_y() + 2);
            break;
        }

        case 6:
        {
            wallPos[1].set_x(wallPos[0].get_x() + 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() + 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() + 2);
            wallPos[3].set_y(wallPos[0].get_y() - 1);
            break;
        }

        case 7:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() - 1);

            wallPos[2].set_x(wallPos[0].get_x());
            wallPos[2].set_y(wallPos[0].get_y() - 2);

            wallPos[3].set_x(wallPos[0].get_x() + 1);
            wallPos[3].set_y(wallPos[0].get_y() - 2);
            break;
        }

        case 8:
        {
            wallPos[1].set_x(wallPos[0].get_x() - 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() - 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() - 2);
            wallPos[3].set_y(wallPos[0].get_y() + 1);
            break;
        }

        //checks if next wall is 'L' piece and the different rotations of said piece
        case 9:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() + 1);

            wallPos[2].set_x(wallPos[0].get_x());
            wallPos[2].set_y(wallPos[0].get_y() + 2);

            wallPos[3].set_x(wallPos[0].get_x() + 1);
            wallPos[3].set_y(wallPos[0].get_y() + 2);
            break;
        }

        case 10:
        {
            wallPos[1].set_x(wallPos[0].get_x() + 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() + 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() + 2);
            wallPos[3].set_y(wallPos[0].get_y() + 1);
            break;
        }

        case 11:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() - 1);

            wallPos[2].set_x(wallPos[0].get_x());
            wallPos[2].set_y(wallPos[0].get_y() - 2);

            wallPos[3].set_x(wallPos[0].get_x() - 1);
            wallPos[3].set_y(wallPos[0].get_y() - 2);
            break;
        }

        case 12:
        {
            wallPos[1].set_x(wallPos[0].get_x() - 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() - 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() - 2);
            wallPos[3].set_y(wallPos[0].get_y() - 1);
            break;
        }

        //checks if next wall is 'O' piece and the different rotations of said piece
        case 13:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() + 1);

            wallPos[2].set_x(wallPos[0].get_x() + 1);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() + 1);
            wallPos[3].set_y(wallPos[0].get_y() + 1);
            break;
        }

        case 14:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() + 1);

            wallPos[2].set_x(wallPos[0].get_x() - 1);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() - 1);
            wallPos[3].set_y(wallPos[0].get_y() + 1);
            break;
        }

        case 15:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() - 1);

            wallPos[2].set_x(wallPos[0].get_x() - 1);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() - 1);
            wallPos[3].set_y(wallPos[0].get_y() - 1);
            break;
        }

        case 16:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() - 1);

            wallPos[2].set_x(wallPos[0].get_x() + 1);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() + 1);
            wallPos[3].set_y(wallPos[0].get_y() - 1);
            break;
        }

        //checks if next wall is 'S' piece and the different rotations of said piece
        case 17:
        {
            wallPos[1].set_x(wallPos[0].get_x() - 1); //0P
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() - 1); //0P
            wallPos[2].set_y(wallPos[0].get_y() + 1); //0

            wallPos[3].set_x(wallPos[0].get_x() - 2); // 0P
            wallPos[3].set_y(wallPos[0].get_y() + 1); //00
            break;
        }

        case 18:
        {
            wallPos[1].set_x(wallPos[0].get_x());     //0
            wallPos[1].set_y(wallPos[0].get_y() - 1); //P

            wallPos[2].set_x(wallPos[0].get_x() + 1); // 00
            wallPos[2].set_y(wallPos[0].get_y() - 1); // P

            wallPos[3].set_x(wallPos[0].get_x() + 1); //  0
            wallPos[3].set_y(wallPos[0].get_y() - 2); // 00
            break;                                    // P
        }

        case 19:
        {
            wallPos[1].set_x(wallPos[0].get_x() + 1); // p0
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() + 1); //  0
            wallPos[2].set_y(wallPos[0].get_y() - 1); // p0

            wallPos[3].set_x(wallPos[0].get_x() + 2); //  00
            wallPos[3].set_y(wallPos[0].get_y() - 1); // p0
            break;
        }

        case 20:
        {
            wallPos[1].set_x(wallPos[0].get_x());     // p
            wallPos[1].set_y(wallPos[0].get_y() + 1); // 0

            wallPos[2].set_x(wallPos[0].get_x() + 1); // p
            wallPos[2].set_y(wallPos[0].get_y() + 1); // 00

            wallPos[3].set_x(wallPos[0].get_x() + 1); // P
            wallPos[3].set_y(wallPos[0].get_y() + 2); // 00
            break;                                    //  0
        }

        //checks if next wall is 'T' piece and the different rotations of said piece
        case 21:
        {
            wallPos[1].set_x(wallPos[0].get_x() + 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() + 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() + 1);
            wallPos[3].set_y(wallPos[0].get_y() + 1);
            break;
        }

        case 22:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() + 1);

            wallPos[2].set_x(wallPos[0].get_x());
            wallPos[2].set_y(wallPos[0].get_y() + 2);

            wallPos[3].set_x(wallPos[0].get_x() + 1);
            wallPos[3].set_y(wallPos[0].get_y() + 1);
            break;
        }

        case 23:
        {
            wallPos[1].set_x(wallPos[0].get_x() - 1);
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() - 2);
            wallPos[2].set_y(wallPos[0].get_y());

            wallPos[3].set_x(wallPos[0].get_x() - 1);
            wallPos[3].set_y(wallPos[0].get_y() - 1);
            break;
        }

        case 24:
        {
            wallPos[1].set_x(wallPos[0].get_x());
            wallPos[1].set_y(wallPos[0].get_y() - 1);

            wallPos[2].set_x(wallPos[0].get_x());
            wallPos[2].set_y(wallPos[0].get_y() - 2);

            wallPos[3].set_x(wallPos[0].get_x() - 1);
            wallPos[3].set_y(wallPos[0].get_y() - 1);
            break;
        }

        //checks if next wall is 'Z' piece and the different rotations of said piece
        case 25:
        {
            wallPos[1].set_x(wallPos[0].get_x() + 1); // p0
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() + 1); // p0
            wallPos[2].set_y(wallPos[0].get_y() + 1); //  0

            wallPos[3].set_x(wallPos[0].get_x() + 2); // p0
            wallPos[3].set_y(wallPos[0].get_y() + 1); //  00
            break;
        }

        case 26:
        {
            wallPos[1].set_x(wallPos[0].get_x());     // p
            wallPos[1].set_y(wallPos[0].get_y() + 1); // 0

            wallPos[2].set_x(wallPos[0].get_x() - 1); //  p
            wallPos[2].set_y(wallPos[0].get_y() + 1); // 00

            wallPos[3].set_x(wallPos[0].get_x() - 1); //  p
            wallPos[3].set_y(wallPos[0].get_y() + 2); // 00
            break;                                    // 0
        }

        case 27:
        {
            wallPos[1].set_x(wallPos[0].get_x() - 1); // 0p
            wallPos[1].set_y(wallPos[0].get_y());

            wallPos[2].set_x(wallPos[0].get_x() - 1); // 0p
            wallPos[2].set_y(wallPos[0].get_y() + 1); // 0

            wallPos[3].set_x(wallPos[0].get_x() - 2); //  0p
            wallPos[3].set_y(wallPos[0].get_y() + 1); // 00
            break;
        }

        case 28:
        {
            wallPos[1].set_x(wallPos[0].get_x());     // 0
            wallPos[1].set_y(wallPos[0].get_y() - 1); // p

            wallPos[2].set_x(wallPos[0].get_x() - 1); // 00
            wallPos[2].set_y(wallPos[0].get_y() - 1); //  p

            wallPos[3].set_x(wallPos[0].get_x() - 1); // 0
            wallPos[3].set_y(wallPos[0].get_y() - 2); // 00
            break;                                    //  p
        }

        default:                                                                                        //checks if next wall is a default 1x1 piece
        {
            return;
        }
    }
}

Position* Wall::getPos(int x)
{
    switch (x)
    {
    case 0:
        return &wallPos[0];

    case 1:
        return &wallPos[1];

    case 2:
        return &wallPos[2];
        
    case 3:
        return &wallPos[3];
    };
}


void Wall::update_pos(double dtime)
{
    //No codes here
}

Position* Wall::new_pos(double dtime)
{
    //No codes here
	Position p;
	return &p;
}