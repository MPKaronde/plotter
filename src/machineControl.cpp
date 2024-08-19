// everything that is run on board the machine
// includes movement code and interpretation code

#include <AccelStepper.h>
#include <math.h>
#define HALFSTEP 4

// motors
AccelStepper X(HALFSTEP, 10, 11, 12, 13);
AccelStepper Y2(HALFSTEP, 6, 7, 8, 9);
AccelStepper Y1(HALFSTEP, 2, 3, 4, 5);
AccelStepper Z(HALFSTEP, 14, 16, 15, 17); // A0 - A3 --> D14 - D17

// axis motor parameters
const int axisSpeed = 1200;
const int maxSpeed = 1200; // 2.0 encoded
const int midSpeed = 1000; // 1.0 encoded
const int lowSpeed = 800;  // 0.0 encoded
const int acceleration = 10000.0;

const double minAcceptableSpeed = 400.0; // minimum safe speed

// z motor parameters
const int zSpeed = 80;
const int zMaxSpeed = 80;
const int zAcceleration = 80;
const double zLiftDistance = 2038 / 3.7;
bool penIsUp;     // whether or not pen is lifted
bool penIsHalfUp; // whether the pen has been half lifted

// current coordinates
double currentX = 0;
double currentY = 0;

// translation parameters
double stepsPerSegment = 4050;
double segmentsPerCm = 1;

// sets all initial motor parameters
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
    penIsHalfUp = false;
}

// sets all machine axis = 0
//
//
void zeroMachine()
{
    currentX = 0;
    currentY = 0;
    Serial.println("CurrentX: " + String(currentX));
    Serial.println("CurrentY: " + String(currentY));
}

// pen height code
// moves pen up when down
//
//
void penUp()
{
    Z.setCurrentPosition(0);
    if (penIsUp)
    {
        Serial.print("error: pen already up");
        return;
    }
    if (penIsHalfUp)
    {
        Z.moveTo(zLiftDistance * 3 / 4);
        while (Z.distanceToGo() != 0)
        {
            Z.run();
        }
        penIsHalfUp = false;
        penIsUp = true;
        return;
    }
    Z.moveTo(zLiftDistance);
    while (Z.distanceToGo() != 0)
    {
        Z.run();
    }
    penIsUp = true;
}
// moves pen down when up
void penDown()
{
    Z.setCurrentPosition(0);
    /*if(!penIsUp or !penIsHalfUp)
    {
        Serial.print("error: pen already down");
        return;
    }*/
    // if pen is half up
    if (penIsHalfUp)
    {
        Z.moveTo(-zLiftDistance / 4);
        while (Z.distanceToGo() != 0)
        {
            Z.run();
        }
        penIsHalfUp = false;
        return;
    }
    // else pen must be all the way up
    Z.moveTo(-zLiftDistance);
    while (Z.distanceToGo() != 0)
    {
        Z.run();
    }
    penIsUp = false;
}
// move the pen up just enough to move
void penHalfLift()
{
    /*
    if(penIsUp or penIsHalfUp)
    {
        Serial.print("error: pen cannot be half raised");
        return;
    }*/
    Z.setCurrentPosition(0);
    Z.moveTo(zLiftDistance / 4);
    while (Z.distanceToGo() != 0)
    {
        Z.run();
    }
    penIsHalfUp = true;
}
// move pen up or down depending on current position
void togglePen()
{
    if (penIsUp)
    {
        penDown();
    }
    if (!penIsUp)
    {
        penDown();
    }
}

// control axis motor speeds
// set the x speed
//
//
void setXspeed(int speed)
{
    X.setSpeed(speed);
    X.setMaxSpeed(speed);
}
// set the y speed
void setYspeed(double speed)
{
    Y1.setSpeed(speed);
    Y1.setMaxSpeed(speed);
    Y2.setSpeed(speed);
    Y2.setMaxSpeed(speed);
}

// code to move individual axis's
// moves the x axis by specified number of motor steps
//
//
void moveXbySteps(double distance, int speed)
{
    setXspeed(speed);

    X.setCurrentPosition(0);
    X.moveTo(distance);

    while (X.distanceToGo() != 0)
    {
        X.run();
    }

    currentX += distance / stepsPerSegment;
    return;
}
// moves the y axis by specified number of motor steps
void moveYbySteps(double distance, int speed)
{
    setYspeed(speed);

    Y1.setCurrentPosition(0);
    Y2.setCurrentPosition(0);
    Y1.moveTo(-distance);
    Y2.moveTo(distance);

    while (Y1.distanceToGo() != 0)
    {
        Y1.run();
        Y2.run();
    }

    currentY += distance / stepsPerSegment;
    return;
}

// move individual axis's by segments
//
//
void moveXBySegments(double distance, int speed)
{
    moveXbySteps(distance * stepsPerSegment, speed);
}
void moveYBySegments(double distance, int speed)
{
    moveYbySteps(distance * stepsPerSegment, speed);
}

// code to move to specified point
// calculates the slope of a line
//
//
double slope(double xDistance, double yDistance)
{
    double top = currentY - abs(yDistance);
    Serial.println(String(currentY) + " - " + String(yDistance) + " = " + String(top));
    double bot = currentX - abs(xDistance);
    Serial.println(String(currentX) + " - " + String(xDistance) + " = " + String(bot));
    // avoid divide by 0
    if (bot == 0)
    {
        return -1;
    }
    return abs(top / bot);
}

//  run to a point in a straight line in terms of segments
void runToPoint(double xPoint, double yPoint)
{
    double s = slope(xPoint, yPoint);
    Serial.println(s);

    // vertical line
    if (s == -1)
    {
        moveYBySegments(yPoint - currentY, maxSpeed);
        return;
    }

    // horizontal line
    if (s == 0)
    {
        moveXBySegments(xPoint - currentX, maxSpeed);
    }

    // s > 1 --> y will run more than x --> reduce x speed
    if (s > 1)
    {
        setYspeed(maxSpeed);
        setXspeed(maxSpeed * (1 / s));
    }

    // s < 1 --> x will run more than y --> reduce y speed
    else if (s < 1)
    {
        setXspeed(maxSpeed);
        setYspeed(maxSpeed * s);
    }

    // assume 45 degrees (s = 1)
    else
    {
        setXspeed(maxSpeed);
        setYspeed(maxSpeed);
    }

    // set location targets
    X.setCurrentPosition(0);
    Y1.setCurrentPosition(0);
    Y2.setCurrentPosition(0);

    X.moveTo((xPoint - currentX) * stepsPerSegment);
    Y1.moveTo((yPoint - currentY) * -stepsPerSegment);
    Y2.moveTo((yPoint - currentY) * stepsPerSegment);

    Serial.println("X Move: " + String(xPoint) + " - " + String(currentX) + " = " + String(xPoint - currentX));
    Serial.println("Y Move: " + String(yPoint) + " - " + String(currentY) + " = " + String(yPoint - currentY));

    // run until can run no more
    while (X.distanceToGo() != 0 or Y1.distanceToGo() != 0 or Y2.distanceToGo() != 0)
    {
        X.run();
        Y1.run();
        Y2.run();
    }

    //Stop as fast as possible to reduce delay
    X.stop();
    Y1.stop();
    Y2.stop();

    // set that have reached location
    currentX = xPoint;
    currentY = yPoint;
}

//  run to a point in a curved line of a constant radius
void curveToPoint(double xPoint, double yPoint)
{

}

// standard patterns to test with
//
//
void makeSquare(double size, double speed)
{
    penDown();
    moveXbySteps(size, speed);
    moveYbySteps(size, speed);
    moveXbySteps(-size, speed);
    moveYbySteps(-size, speed);
    penUp();
}
void writeHello()
{
    // H
    penDown();
    moveYbySteps(5000, maxSpeed);
    moveYbySteps(-2500, maxSpeed);
    moveXbySteps(2500, maxSpeed);
    moveYbySteps(-2500, maxSpeed);
    moveYbySteps(5000, maxSpeed);
    penHalfLift();

    // reposition
    moveXbySteps(1000, maxSpeed);

    // E
    penDown();
    moveYbySteps(-5000, maxSpeed);
    moveXbySteps(2500, maxSpeed);
    moveXbySteps(-2500, maxSpeed);
    moveYbySteps(2500, maxSpeed);
    moveXbySteps(2500, maxSpeed);
    moveXbySteps(-2500, maxSpeed);
    moveYbySteps(2500, maxSpeed);
    moveXbySteps(2500, maxSpeed);
    penHalfLift();

    // reposition
    moveXbySteps(1000, maxSpeed);

    // L
    penDown();
    moveYbySteps(-5000, maxSpeed);
    moveXbySteps(2500, maxSpeed);
    penHalfLift();

    // reposition
    moveXbySteps(1000, maxSpeed);
    moveYbySteps(5000, maxSpeed);

    // L
    penDown();
    moveYbySteps(-5000, maxSpeed);
    moveXbySteps(2500, maxSpeed);
    penHalfLift();

    // reposition
    moveXbySteps(1000, maxSpeed);
    moveYbySteps(5000, maxSpeed);

    // O
    penDown();
    moveXbySteps(2500, maxSpeed);
    moveYbySteps(-5000, maxSpeed);
    moveXbySteps(-2500, maxSpeed);
    moveYbySteps(5000, maxSpeed);
    penUp();
}

// gets keyboard input to start loaded code
//
//
void onStart()

{
    bool start = false;
    while (!start)
    {
        Serial.print("Do you wish to begin the program? y/n:\n");
        String input = Serial.readString();
        if (input == "y")
        {
            start = true;
        }
        Serial.println();
        delay(100);
    }
}

// correct to the right speed
double setAcceptableSpeed(double speed)
{
    // if its lower than min acceptable speed, its either a preset or make it min acceptable speed
    if (speed < minAcceptableSpeed)
    {
        if (speed == 0.0)
        {
            speed = lowSpeed;
        }
        else if (speed == 1.0)
        {
            speed = midSpeed;
        }
        else if (speed == 2.0)
        {
            speed = maxSpeed;
        }
        else
        {
            speed = minAcceptableSpeed;
        }
    }
    // if speed is above safe max speed
    else if (speed > maxSpeed)
    {
        speed = maxSpeed;
    }
    return speed;
}

// code to process commands from computer read code
bool processCommand(String command)
{
    int space1 = command.indexOf(" ");
    String main;

    if (space1 == -1)
    {
        int len = command.length();
        // not having a space makes /n become part of the command, so must remove
        main = command.substring(0, len - 1);
    }
    else
    {
        main = command.substring(0, space1);
    }

    // if progam is to end
    if (main == "end")
    {
        return true;
    }

    // moveByX
    if (main == "x")
    {
        int space2 = command.indexOf(" ", space1 + 1);
        String dist = command.substring(space1, space2);
        double distance = dist.toDouble();
        double speed = command.substring(space2).toDouble();

        speed = setAcceptableSpeed(speed);

        moveXbySteps(distance, speed);
    }

    // moveByX in terms of segments
    if (main == "xbs")
    {
        int space2 = command.indexOf(" ", space1 + 1);
        String dist = command.substring(space1, space2);
        double distance = dist.toDouble();
        double speed = command.substring(space2).toDouble();

        speed = setAcceptableSpeed(speed);

        moveXBySegments(distance, speed);
        Serial.println("understood");
    }

    // move by y in terms of segments
    if (main == "ybs")
    {
        int space2 = command.indexOf(" ", space1 + 1);
        String dist = command.substring(space1, space2);
        double distance = dist.toDouble();
        double speed = command.substring(space2).toDouble();

        speed = setAcceptableSpeed(speed);

        moveYBySegments(distance, speed);
    }

    // moveByY
    if (main == "y")
    {
        int space2 = command.indexOf(" ", space1 + 1);
        String dist = command.substring(space1, space2);
        double distance = dist.toDouble();
        double speed = command.substring(space2).toDouble();

        speed = setAcceptableSpeed(speed);

        moveYbySteps(distance, speed);
    }

    // toggle pen
    if (main == "tp")
    {
        togglePen();
    }

    // pen up
    if (main == "pu")
    {
        penUp();
    }

    // pen down
    if (main == "pd")
    {
        penDown();
    }

    // pen half up
    if (main == "hu")
    {
        penHalfLift();
    }

    // run to point
    if (main == "rtp")
    {
        int space2 = command.indexOf(" ", space1 + 1);

        double x = command.substring(space1, space2).toDouble();
        double y = command.substring(space2).toDouble();

        runToPoint(x, y);
    }

    // set x speed
    if (main == "xs")
    {
        double speed = command.substring(space1).toDouble();

        setXspeed(speed);
    }

    // set y speed
    if (main == "ys")
    {
        double speed = command.substring(space1).toDouble();

        setYspeed(speed);
    }

    // draw square
    if (main == "square")
    {
        int space2 = command.indexOf(" ", space1 + 1);
        double distance = command.substring(space1, space2).toDouble();
        double speed = command.substring(space2).toDouble();

        makeSquare(distance, speed);
    }

    // draw hello
    if (main == "hello")
    {
        writeHello();
    }

    // zero machine
    if (main == "z")
    {
        zeroMachine();
    }

    // allows next command
    return false;
}

// handles all serial communication
void runProgam()
{
    bool end = false;

    int i = 1;

    while (end == false)
    {
        // send confirmation that ready for next command
        Serial.println("go " + String(i));
        String command = Serial.readString();
        end = processCommand(command);
        i += 1;
    }
}
// runner loop
//
//
bool complete = false;
void loop()
{
    /*
    Serial.print("\n");
    while (1 == 1)
    {
        Serial.print("hello\n");
    }
    */
    // put all runner code within if statement
    if (!complete)
    {
        // Serial.print("hello");

        onStart();

        // Start all runner code here
        runProgam();

        // end all runner code before here
        complete = true;
    }
    else
    {
        Serial.print("done");
        Serial.end();
        return;
    }
}
