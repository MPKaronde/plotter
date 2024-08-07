
#include <AccelStepper.h>
#include <math.h>

#include "grid.h"

using namespace G;

//speed 0 = superLow, 1 = reallyLow, 2 = slower, 3 = max
//methods to move x by different units of measure
void Grid::moveXBySteps(double steps, int speed)
{
    X.move(steps);

}
void Grid::moveXBySegments(double segments, int speed)
{

}
void Grid::moveXByCM(double distance, int speed)
{

}

//methods to move y by different units of measure
void Grid::moveYBySteps(double steps, int speed)
{

}
void Grid::moveYBySegments(double steps, int speed)
{

}
void Grid::moveYByCM(double steps, int speed)
{

}