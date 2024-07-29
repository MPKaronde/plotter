//Basic level movement code
#include <AccelStepper.h>
#include <runners.h>
#include <math.h>

using namespace N;

void grid::zeroAxis()
{
    int xSwitch = 0;
    int ySwitch = 0;
    int zSwitch = 0;

    //zero x axis
    while (xSwitch == 0)
    {
        x.setSpeed(STANDARD_SPEED);
        x.run();
        xSwitch = digitalRead(X_BUMP_PIN);
    }
    x.stop();
    currentMachineX = 0;

    //zero y axis
    while (ySwitch == 0)
    {
        y.setSpeed(STANDARD_SPEED);
        y.run();
        ySwitch = digitalRead(Y_BUMP_PIN);
    }
    y.stop();
    currentMachineY = 0;

    //zero z axis
    while (zSwitch == 0)
    {
        z.setSpeed(STANDARD_SPEED);
        z.run();
        zSwitch = digitalRead(Z_BUMP_PIN);
    }
    z.stop();
    penRaised = true;
}

void grid::runTo(int xTarget, int yTarget)
{
    int xToGo = currentMachineX - xTarget;
    int yToGo = currentMachineX - yTarget;

    x.moveTo(xToGo * STEPS_PER_SEGMENT);
    y.moveTo(yToGo * STEPS_PER_SEGMENT);

    currentMachineX = xTarget;
    currentMachineY = yTarget;
}

void grid::runByLine(double xTarget, double yTarget)
{
    double xToGo = currentMachineX - xTarget;
    double yToGo = currentMachineX - yTarget;

    //emulate a straight line by first moving x by a small amount, then y by a small amount over and over
    while(xToGo != 0 || yToGo != 0)
    {
        //move the x by a small segment
        x.moveTo(xToGo * MIN_SEGMENT_AMT * STEPS_PER_SEGMENT);
        if(xToGo > 0)
        {
            xToGo -= xToGo * MIN_SEGMENT_AMT;
        }
        else if (xToGo < 0)
        {
            xToGo += xToGo * MIN_SEGMENT_AMT;
        }

        //move the y by a small segment
        y.moveTo(yToGo * MIN_SEGMENT_AMT * STEPS_PER_SEGMENT);
        if(yToGo > 0)
        {
            yToGo -= yToGo * MIN_SEGMENT_AMT;
        }
        else if (yToGo < 0)
        {
            yToGo += yToGo * MIN_SEGMENT_AMT;
        }  
    }

    currentMachineX = xTarget;
    currentMachineY = yTarget;
}

//starts from leftmost point, drawing the top, then goes from rightmost point drawing the bottom
void grid::makeCircle(int vertexX, int vertexY, int radius)
{
    //move to start point (furthest left point)
    runByLine(vertexX - radius, vertexY);

    //draw the top section
    while (currentMachineX < vertexX + radius)
    {
        //where we want to go with x
        int targetX = currentMachineX + MIN_SEGMENT_AMT;
        
        //calculate where we want to go with y
        double targetY = pow(radius, 2) - pow((currentMachineX - vertexX), 2);
        targetY = sqrt(targetY);
        targetY += currentMachineY;
        int tY = round(targetY * pow(10, MIN_SEGMENT_AMT)) / pow(10, MIN_SEGMENT_AMT);

        runByLine(targetX, tY);
    }

    //draw the bottom section
    while (currentMachineX > vertexX - radius)
    {
        //where we want to go with x
        int targetX = currentMachineX - MIN_SEGMENT_AMT;
        
        //calculate where we want to go with y
        double targetY = pow(radius, 2) - pow((currentMachineX - vertexX), 2);
        targetY = sqrt(targetY);
        targetY += currentMachineY;
        int tY = round(targetY * pow(10, MIN_SEGMENT_AMT)) / pow(10, MIN_SEGMENT_AMT) * -1;

        runByLine(targetX, tY);
    }
}

void grid::togglePen()
{
    if (penRaised == true)
    {
        z.moveTo(-1 * Z_MOVE_DISTANCE);
    }
    else
    {
        z.moveTo(Z_MOVE_DISTANCE);
    }
}