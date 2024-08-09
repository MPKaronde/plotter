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
const int maxSpeed = 1200;
const int midSpeed = 1000;
const int lowSpeed = 800; 
const int acceleration = 10000.0;

//z motor parameters
const int zSpeed = 100;
const int zMaxSpeed = 100;
const int zAcceleration = 80;
const double zLiftDistance = 2038 / 5;
bool penIsUp; //whether or not pen is lifted

//current coordinates
double currentX = 0;
double currentY = 0;

//translation parameters
double stepsPerSegment = 1;
double segmentsPerCm = 1;


//sets all initial motor parameters
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

    penIsUp = true;
}


//pen height code
//moves pen up when down
void penUp()
{
    if(penIsUp)
    {
        Serial.print("error: pen already up");
        return;
    }
    Z.moveTo(2038/3.8);
    while(Z.distanceToGo() != 0)
    {
        Z.run();
    }
    penIsUp = true;
}
//moves pen down when up
void penDown()
{
    if(!penIsUp)
    {
        Serial.print("error: pen already down");
        return;
    }
    Z.moveTo(-2038/3.8);
    while(Z.distanceToGo() != 0)
    {
        Z.run();
    }
    penIsUp = false;
}
//move pen up or down depending on current position
void togglePen()
{
    if(penIsUp)
    {
        penDown();
    }
    if(!penIsUp)
    {
        penDown();
    }
}


//control axis motor speeds
//set the x speed 
void setXspeed(int speed)
{
    X.setSpeed(speed);
    X.setMaxSpeed(speed);
}
//set the y speed
void setYspeed(double speed)
{
    Y1.setSpeed(speed);
    Y1.setMaxSpeed(speed);
    Y2.setSpeed(speed);
    Y2.setMaxSpeed(speed);
}


//code to move individual axis's
//moves the x axis by specified number of motor steps
void moveXbySteps(double distance, int speed)
{
    setXspeed(speed);

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
    Y1.moveTo(-distance);
    Y2.moveTo(distance);

    while(Y1.distanceToGo() != 0)
    {
        Y1.run();
        Y2.run();
    }

    currentY += distance;
    return;
}


//code to move to specified point
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
        setYspeed(maxSpeed);
        setXspeed(maxSpeed * (1 / s));
    }

    //s < 1 --> x will run more than y --> reduce y speed
    else if(s < 1)
    {
        setXspeed(maxSpeed);
        setYspeed(maxSpeed * s);
    }

    //assume 45 degrees (s = 1)
    else 
    {
        setXspeed(maxSpeed);
        setYspeed(maxSpeed);
    }

    //set location targets
    X.setCurrentPosition(0);
    Y1.setCurrentPosition(0);
    Y2.setCurrentPosition(0);

    X.moveTo((xPoint - currentX) * stepsPerSegment);
    Y1.moveTo((yPoint - currentY) *  -stepsPerSegment);
    Y2.moveTo((yPoint - currentY) * stepsPerSegment);

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
    setXspeed(maxSpeed);
    setYspeed(maxSpeed);
}


//runner loop
bool complete = false;
void loop()
{
    //put all runner code within if statement
    if(!complete)
    {   
        //Start all runner code here
        runToPoint(10000, 10000);

        //end all runner code before here
        complete = true;
    }
    else
    {
        Serial.print("done");
        Serial.end();
        return;
    }
}
