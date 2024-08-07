#include <AccelStepper.h>
#include <math.h>

#include "grid.h"

using namespace G;

//define all steppers
AccelStepper X(HALFSTEP, 10, 11, 12, 13);
AccelStepper Y1(HALFSTEP, 2, 3, 4, 5);
AccelStepper Y2(HALFSTEP, 6, 7, 8, 9);
AccelStepper Z(HALFSTEP, 14, 15, 26, 17); //A0 - A3 --> D14 - D17
Grid g(Y1, Y2, X, Z);

void setup()
{
    X.setMaxSpeed(800);
    X.setSpeed(800);
    X.setAcceleration(10000.0);
    X.moveTo(-5000);
}

void loop()
{
    X.run();
}

//check if the motors have gone the full distance they needed to in their previous command and if not then run them
void runMotors()
{
    while(X.distanceToGo() != 0 or Y1.distanceToGo() != 0 or Y2.distanceToGo() != 0 or Z.distanceToGo() != 0)
    {
        X.run();
        Y1.run();
        Y2.run();
        Z.run();
    }
}
