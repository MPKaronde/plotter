#include <AccelStepper.h>
#include <math.h>

namespace A
{
    class Grid //the machine is split into a divisible 10 x 10 grid
    {
        const int STEPS_PER_SEGMENT = 1; // how many motor steps are needed for each segment
        const int SEGMENT_PER_INCH = 1; // how many segments are in each inch for when getting sizing
        const int STANDARD_SPEED = 1; // the speed everything else will be relative to
        const int X_BUMP_PIN = 1; //pin for x bump switch
        const int Y_BUMP_PIN = 2; //pin for y bump switch
        const int Z_BUMP_PIN = 3; //pin for z bump switch
        const double MIN_SEGMENT_AMT = 1 / 100; //the smallest amount by which the machine will attempt to move
        const int Z_MOVE_DISTANCE = 1; //the amount the z motor must move to bring the pen all the way up or down
        const int AXIS_ACCELERATION = 50; //speed at which the axis will accelerate
        const int Z_ACCELERATION = 50; //speed at which z stepper will accelerate

        public:
            //the motors
            AccelStepper x;
            AccelStepper y;
            AccelStepper z;

            //current machine coordinates
            int currentX;
            int currentY;
            bool penIsDown;

            //constructor
            Grid(AccelStepper xIn, AccelStepper yIn, AccelStepper zIn)
            {
                x = xIn;
                y = yIn;
                z = zIn;

                x.setAcceleration(AXIS_ACCELERATION);
                y.setAcceleration(AXIS_ACCELERATION);
                z.setAcceleration(Z_ACCELERATION)
            }

            //Zeros the axis
            void zeroAxis(){}

            //toggles the pen up and down
            void togglePen(){}

            //runs machine in a segment from current location to new location
            void move(int xTarget, int yTarget){}

            //find slope of line
            double findSlope(int x1, int x2, int y1, int y2){}

            //round input value to have number of places of min segment val
            double roundToMinSegment(double in){}

    }
}