#include <AccelStepper.h>
#include <math.h>
#define HALFSTEP 4

//motors
AccelStepper X(HALFSTEP, 10, 11, 12, 13);
AccelStepper Y2(HALFSTEP, 6, 7, 8, 9);
AccelStepper Y1(HALFSTEP, 2, 3, 4, 5);
AccelStepper Z(HALFSTEP, 14, 16, 15, 17); //A0 - A3 --> D14 - D17

//axis motor parameters
const int axisSpeed = 1200;
const int maxSpeed = 1200; //speed 2
const int midSpeed = 1000; //speed 1
const int lowSpeed = 800; //speed 0
const int acceleration = 10000.0;

//z motor parameters
const int zSpeed = 100;
const int zMaxSpeed = 100;
const int zAcceleration = 80;
const double zLiftDistance = 2038 / 5;

//current coordinates
double currentX = 0;
double currentY = 0;

//translation parameters
double stepsPerSegment = 1;
double segmentsPerCm = 1;

void setup()
{
    Serial.begin(9600);

    X.setMaxSpeed(maxSpeed);
    X.setSpeed(axisSpeed);
    X.setAcceleration(acceleration);

    Y1.setMaxSpeed(maxSpeed);
    Y1.setSpeed(axisSpeed);
    Y1.setAcceleration(acceleration);

    Y2.setMaxSpeed(maxSpeed);
    Y2.setSpeed(axisSpeed);
    Y2.setAcceleration(acceleration);

    Z.setMaxSpeed(zMaxSpeed);
    Z.setSpeed(zSpeed);
    Z.setAcceleration(zAcceleration);
}

//moves pen up when down
void penUp()
{
    Z.moveTo(2038/3.8);
    while(Z.distanceToGo() != 0)
    {
        Z.run();
    }
}
//moves pen down when up
void penDown()
{
    Z.moveTo(-2038/3.8);
    while(Z.distanceToGo() != 0)
    {
        Z.run();
    }
}

//set the x speed 
void setXspeed(int speed)
{
    if(speed == 0)
    {
        X.setSpeed(800);
    }
    else if(speed == 1)
    {
        X.setSpeed(1000);
    }
    else if(speed == 2)
    {
        X.setSpeed(1200);
    }
    else
    {
        Serial.print("invalid speed");
        return;
    }
}
//set the y speed
void setYspeed(int speed)
{
    if(speed == 0)
    {
        Y1.setSpeed(800);
        Y1.setMaxSpeed(800);
        Y2.setSpeed(800);
        Y2.setMaxSpeed(800);
    }
    else if(speed == 1)
    {
        Y1.setSpeed(1000);
        Y1.setMaxSpeed(1000);
        Y2.setSpeed(1000);
        Y2.setMaxSpeed(1000);
    }
    else if(speed == 2)
    {
        Y1.setSpeed(1200);
        Y1.setMaxSpeed(1200);
        Y2.setSpeed(1200);
        Y2.setMaxSpeed(1200);
    }
    else
    {
        Serial.print("invalid speed");
        return;
    }
}

//moves the x axis by specified number of motor steps
void moveXbySteps(double distance, int speed)
{
    //setXspeed(speed);
    X.setSpeed(1000);
    X.setCurrentPosition(0);
    X.moveTo(distance);
    while(X.distanceToGo() != 0)
    {
        X.run();
    }
    currentX += distance;
    return;

}
//moves the y axis by specified number of motor steps
void moveYbySteps(double distance, int speed)
{
    setYspeed(speed);
    Y1.setCurrentPosition(0);
    Y2.setCurrentPosition(0);
    Y1.move(distance);
    Y2.move(-distance);
    while(Y1.distanceToGo() != 0)
    {
        Y1.run();
        Y2.run();
    }
    currentY += distance;
    return;
}

//calculates the slope of a line
double slope(double xDistance, double yDistance)
{
    double top = currentY - yDistance;
    double bot = currentX - xDistance;
    return abs(top / bot);
}
//run to a point in a straight line
void runToPoint(double xPoint, double yPoint)
{
    double s = slope(xPoint, yPoint);

    Serial.print(s);
    //s > 1 --> y will run more than x --> reduce x speed
    if(s > 1)
    {
        Y1.setSpeed(maxSpeed);
        Y1.setMaxSpeed(maxSpeed);

        Y2.setSpeed(maxSpeed);
        Y2.setMaxSpeed(maxSpeed);

        X.setSpeed(maxSpeed * (1 / s));
        X.setMaxSpeed(maxSpeed * (1 / s));

        Serial.println(Y1.speed());
        Serial.println(Y2.speed());
        Serial.println(X.speed());
    }

    //s < 1 --> x will run more than y --> reduce y speed
    else if(s < 1)
    {
        X.setSpeed(maxSpeed);
        X.setMaxSpeed(maxSpeed);

        Y1.setSpeed(maxSpeed * s);
        Y1.setMaxSpeed(maxSpeed * s);

        Y2.setSpeed(maxSpeed * s);
        Y2.setMaxSpeed(maxSpeed * s);

        Serial.println(Y1.speed());
        Serial.println(Y2.speed());
        Serial.println(X.speed());
    }

    else 
    {
        X.setSpeed(maxSpeed);
        Y1.setSpeed(maxSpeed);
        Y2.setSpeed(maxSpeed);
        Serial.println(Y1.speed());
        Serial.println(Y2.speed());
        Serial.println(X.speed());
    }

    //set location targets
    X.setCurrentPosition(0);
    Y1.setCurrentPosition(0);
    Y2.setCurrentPosition(0);

    X.moveTo((xPoint - currentX) * stepsPerSegment);
    Y1.moveTo((yPoint - currentY) *  stepsPerSegment);
    Y2.moveTo((yPoint - currentY) * -stepsPerSegment);

    //run until can run no more
    while(X.distanceToGo() != 0 or Y1.distanceToGo() != 0 or Y2.distanceToGo() != 0)
    {
        X.run();
        Y1.run();
        Y2.run();
    }

    //set that have reached location
    currentX = xPoint;
    currentY = yPoint;

    //reset motor speeds
    setXspeed(2);
    X.setMaxSpeed(maxSpeed);
    setYspeed(2);
    Y1.setMaxSpeed(maxSpeed);
    Y2.setMaxSpeed(maxSpeed);
}

bool complete = false;

void loop()
{
   
    if(!complete)
    {
        //penUp();
        //penDown();
        //delay(1000);
        //runToPoint(-5000, 10000);
        //moveYbySteps(10000, 0);
/*
        Y1.setSpeed(600);
        Y2.setSpeed(600);
        X.setSpeed(1000);

        X.setCurrentPosition(0);
        Y1.setCurrentPosition(0);
        Y2.setCurrentPosition(0);

        X.moveTo(10000);
        Y1.moveTo(-5000);
        Y2.moveTo(5000);

        while(X.distanceToGo() != 0)
        {
            X.run();
            Y1.run();
            Y2.run();
        }
    */
        Y2.setCurrentPosition(0);
        Y2.setMaxSpeed(1000);
        Y2.moveTo(10000);
        while(1 == 1)
        {
            Y2.run();
        }

        complete = true;
    }
    else
    {
        Serial.print("done");
        Serial.end();
    }

    
}
