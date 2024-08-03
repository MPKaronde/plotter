#include <AccelStepper.h>
#include <math.h>
#include <rrunner.h>

using namespace A;

//zeros out the axis
void Grid::zeroAxis()
{
    int xSwitch = 0;
    int ySwitch = 0;
    int zSwitch = 0;

    //zero x axis
    while(xSwitch == 0)
    {
        x.setSpeed(STANDARD_SPEED / 2);
        x.run();
        xSwitch = digitalRead(X_BUMP_PIN);
    }
    currentX = 0;

    //zero y axis
    while(xSwitch == 0)
    {
        y.setSpeed(STANDARD_SPEED / 2);
        y.run();
        ySwitch = digitalRead(Y_BUMP_PIN);
    }
    currentY = 0;

    //zero z (pen) axis
    while(zSwitch == 0)
    {
        z.setSpeed(STANDARD_SPEED / 2);
        z.run();
        zSwitch = digitalRead(Z_BUMP_PIN);
    }
    penIsDown = false;
}

//raise or lower the pen
void Grid::togglePen()
{
    if(penIsDown)
    {
        z.moveTo(-1 * Z_MOVE_DISTANCE);
        penIsDown = false;
    }
    else
    {
        z.moveTo(Z_MOVE_DISTANCE);
        penIsDown = true;
    }
    z.run();
}

//move from current location to given location
void Grid::move(int xTarget, int yTarget)
{
    //error check
    if (xTarget > NUM_SEGMENTS_X or yTarget > NUM_SEGMENTS_Y)
    {
        Serial.print("ERROR OUT OF BOUNDS DISTANCE INPUT");
        return;
    }

    /*if slope abs value is greater than 1, y will move more than x so for every one segment move of y,
    should have fractional movement of x and vice versa for if slope abs value less than 1*/

    double slope = findSlope(currentX, xTarget, currentY, yTarget);

    //move y more than x
    if(abs(slope) > 1)
    {
        //use x as marker for when to end in case y doesnt round perfectly for some reason
        while(currentX != xTarget)
        {
            runYBySegment(slope);
            currentY += slope;

            runXBySegment(MIN_SEGMENT_AMT);
            currentX += MIN_SEGMENT_AMT;
        }
    }
    //move x more than y
    else if(abs(slope) < 1)
    {
        //use y as marker for when to end in case x doesnt round perfectly for some reason
        while(currentY != yTarget)
        {
            runXBySegment(slope);
            currentX += slope;

            runYBySegment(MIN_SEGMENT_AMT);
            currentY += MIN_SEGMENT_AMT;
        }
    }
    //move both equal amounts
    else
    {
        //marker here doesnt really matter bc both are being run by MIN_SEGMENT_AMT
        while(currentY != yTarget)
        {
            runXBySegment(MIN_SEGMENT_AMT);
            currentX += MIN_SEGMENT_AMT;

            runYBySegment(MIN_SEGMENT_AMT);
            currentY += MIN_SEGMENT_AMT;
        }
    }
}

//find the slope between two points
double Grid::findSlope(int x1, int x2, int y1, int y2)
{
    double top = y2 - y1;
    double bot = x2 - x1;

    return roundToMinSegment(top / bot);
}

//rounds given value to number of places in min segment
double Grid::roundToMinSegment(double in)
{
    double toRound = in * pow(10, 1 / MIN_SEGMENT_AMT);
    toRound = round(toRound);
    return toRound / pow(10, 1 / MIN_SEGMENT_AMT);
}

void Grid::runYBySegment(double numSegments)
{
    //error check
    if (currentY + numSegments > NUM_SEGMENTS_Y)
    {
        Serial.print("ILLEGAL DISTANCE");
        return;
    }

    y.move(numSegments * STEPS_PER_SEGMENT);
    y.run();
}

void Grid::runXBySegment(double numSegments)
{
    //error check
    if (currentX + numSegments > NUM_SEGMENTS_X)
    {
        Serial.print("ILLEGAL DISTANCE");
        return;
    }

    x.move(numSegments * STEPS_PER_SEGMENT);
    x.run();
}