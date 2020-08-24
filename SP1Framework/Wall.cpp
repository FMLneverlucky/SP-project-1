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
    wallPivot.set_x(x);                                                          //set x coordinate of variable, wallPivotPoint, as a number from 0 to 80
    wallPivot.set_y(y);                                                          //set y coordinate of variable, wallPivotPoint, as a number from 0 to 24
}

void Wall::setPosForAll()
{
    int x = rand() % 8;
    switch (x)                                                                                  //checks if the next wall type is a 'I' or line piece
    {
        case 1:
        {
            wall2.set_x(wallPivot.get_x());                                                        //setting x coord of second wall using previous pivot point as reference
            wall2.set_y(wallPivot.get_y() + 1);                                                    //setting y coord of second wall using previous pivot point as reference

            wall3.set_x(wallPivot.get_x());                                                        //setting x coord of third wall using previous pivot point as reference
            wall3.set_y(wallPivot.get_y() + 2);                                                    //setting y coord of third wall using previous pivot point as reference

            wall4.set_x(wallPivot.get_x());                                                        //setting x coord of fourth wall using previous pivot point as reference
            wall4.set_y(wallPivot.get_y() + 3);                                                    //setting y coord of fourth wall using previous pivot point as reference
        }

        case 2:                                                                                         //checks if next wall is 'J' piece
        {
            wall2.set_x(wallPivot.get_x());
            wall2.set_y(wallPivot.get_y() + 1);

            wall3.set_x(wallPivot.get_x());
            wall3.set_y(wallPivot.get_y() + 2);

            wall4.set_x(wallPivot.get_x() - 1);
            wall4.set_y(wallPivot.get_y() + 2);
        }

        case 3:                                                                                         //checks if next wall is 'L' piece
        {
            wall2.set_x(wallPivot.get_x());
            wall2.set_y(wallPivot.get_y() + 1);

            wall3.set_x(wallPivot.get_x());
            wall3.set_y(wallPivot.get_y() + 2);

            wall4.set_x(wallPivot.get_x() + 1);
            wall4.set_y(wallPivot.get_y() + 2);
        }

        case 4:                                                                                         //checks if next wall is 'O' piece
        {
            wall2.set_x(wallPivot.get_x());
            wall2.set_y(wallPivot.get_y() + 1);

            wall3.set_x(wallPivot.get_x() + 1);
            wall3.set_y(wallPivot.get_y());

            wall4.set_x(wallPivot.get_x() + 1);
            wall4.set_y(wallPivot.get_y() + 1);
        }

        case 5:                                                                                         //checks if next wall is 'S' piece
        {
            wall2.set_x(wallPivot.get_x() - 1);
            wall2.set_y(wallPivot.get_y());

            wall3.set_x(wallPivot.get_x() - 1);
            wall3.set_y(wallPivot.get_y() + 1);

            wall4.set_x(wallPivot.get_x() - 2);
            wall4.set_y(wallPivot.get_y() + 1);
        }

        case 6:                                                                                         //checks if next wall is 'T' piece
        {
            wall2.set_x(wallPivot.get_x() + 1);
            wall2.set_y(wallPivot.get_y());

            wall3.set_x(wallPivot.get_x() + 2);
            wall3.set_y(wallPivot.get_y());

            wall4.set_x(wallPivot.get_x() + 1);
            wall4.set_y(wallPivot.get_y() + 1);
        }

        case 7:                                                                                         //checks if next wall is 'z' piece
        {
            wall2.set_x(wallPivot.get_x() + 1);
            wall2.set_y(wallPivot.get_y());

            wall3.set_x(wallPivot.get_x() + 1);
            wall3.set_y(wallPivot.get_y() + 1);

            wall4.set_x(wallPivot.get_x() + 2);
            wall4.set_y(wallPivot.get_y() + 1);
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
    case 1:
        return &wallPivot;

    case 2:
        return &wall2;

    case 3:
        return &wall3;
        
    case 4:
        return &wall4;
    };
}
//int Wall::setType()
//{
//	int x = rand() % 8;
//	int nextWall;
//	switch (x)
//	{
//	case 1:
//		nextWall = I;
//		break;
//	case 2:
//		nextWall = J;
//		break;
//	case 3:
//		nextWall = L;
//		break;
//	case 4:
//		nextWall = O;
//		break;
//	case 5:
//		nextWall = S;
//		break;
//	case 6:
//		nextWall = T;
//		break;
//	case 7:
//		nextWall = Z;
//		break;
//	default:
//		nextWall = d;
//	}
//	return nextWall;
//}

//void Wall::typeI()
//{
//	
//}
//
//void Wall::typeJ()
//{
//
//}
//
//void Wall::typeL()
//{
//
//}
//
//void Wall::typeO()
//{
//
//}
//
//void Wall::typeS()
//{
//
//}
//
//void Wall::typeT()
//{
//
//}
//
//void Wall::typeZ()
//{
//
//}
//
//void Wall::typed()
//{
//
//}

void Wall::update_pos(double dtime)
{

}

Position* Wall::new_pos(double dtime)
{
	Position p;
	return &p;
}