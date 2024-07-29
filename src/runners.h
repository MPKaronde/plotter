#include <AccelStepper.h>

namespace N
{
    class grid
    {
        const int STEPS_PER_SEGMENT = 1; //number of steps motor must go for each 1/100 grid segment
        const int SEGMENT_PER_INCHES = 1; //number of segments in each inch for when want to be precise abt sizes
        const int STANDARD_SPEED = 1; //speed at which all normal operations are run
        const int X_BUMP_PIN = 1; //pin for x bump switch
        const int Y_BUMP_PIN = 2; //pin for y bump switch
        const int Z_BUMP_PIN = 3; //pin for z bump switch
        const double MIN_SEGMENT_AMT = 1 / 100; //the smallest amount by which the machine will attempt to move
        const int Z_MOVE_DISTANCE = 1; //the amount the z motor must move to bring the pen all the way up or down

        public:
            AccelStepper x;
            AccelStepper y;
            AccelStepper z;

            //current coordinates of the head with respect to the machine
            double currentMachineX;
            double currentMachineY;
            double penRaised;


            grid(AccelStepper inx, AccelStepper iny, AccelStepper inz)
            {
                x = inx;
                y = iny;
                z = inz;
            }

            //zeros out the axises and sets machine coordinates to 0
            void zeroAxis(){}

            //run from current location to a given location first by x, then by y
            void runTo(int xTarget, int yTarget){}

            //run from current location to given location in a straight line
            void runByLine(double xTarget, double yTarget){}

            //create a circle centered at given point of given radius
            void makeCircle(int vertexX, int vertexY, int radius){}

            //move the pen to the position it currently isnt in
            void togglePen(){}

            //return if the pen is raised or not
            boolean isPenRaised()
            {
                return penRaised;
            }
    };
}