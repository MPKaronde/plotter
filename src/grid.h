#ifndef _GRID_H_INCLUDED
#define _GRID_H_INCLUDED

#include <AccelStepper.h>
#include <math.h>

namespace G
{
    class Grid //machine is split into 10 x 10 grid
    {
        //Preset possible speeds
        const double MAX_SPEED = 1200.0; //max attemptable speed
        const double SLOWER_SPEED = 1000.0; //for when less speed is neccesary
        const double REALLY_LOW_SPEED = 800.0; //for when less speed is super neccesary
        const double SUPER_LOW_SPEED = 600.0; //lowest attemptable speed
        
        //Other motor presets
        const double AXIS_ACCELERATION = 10000.0; //acceleration for each motor
        #define HALFSTEP 4

        //Distance presets
        const int STEPS_PER_SEGMENT = 1; //how many motor steps equate to 1 segment (1 / 10 of the total possible usable distance)
        const double SEGMENTS_PER_CM = 1.0; //how many segments (1/10 of the grid) is in a cm

        public:
            //Motor Definition
            AccelStepper Y1;
            AccelStepper Y2;
            AccelStepper X;
            AccelStepper Z;

            //Constructor
            Grid(AccelStepper y1, AccelStepper y2, AccelStepper x, AccelStepper z)
            {
                //Motor initialization
                Y1 = y1;
                Y2 = y2;
                X = x;
                Z = z;

                //Set parameters
                Y1.setMaxSpeed(MAX_SPEED);
                Y1.setAcceleration(AXIS_ACCELERATION);
                Y2.setMaxSpeed(MAX_SPEED);
                Y2.setAcceleration(AXIS_ACCELERATION);
                X.setMaxSpeed(MAX_SPEED);
                X.setAcceleration(AXIS_ACCELERATION);
            }

            //methods to move xAxis alone by different units of measure
            void moveXBySteps(double steps, int speed);
            void moveXBySegments(double segments, int speed);
            void moveXByCM(double cm, int speed);

            //methods to move yAxis alone by different units of measure
            void moveYBySteps(double steps, int speed);
            void moveYBySegments(double segments, int speed);
            void moveYByCM(double segments, int speed);
    };
}

#endif // _GRID_H_INCLUDED
