//parses the machineCode and runs it
#include <AccelStepper.h>
#include <math.h>
#include <rrunner.h>

using namespace A;

//setup code in arduino
void setup()
{
    Serial.begin(9600);
}

//object from rrunner files
Grid grid;

//code runner in arduino uses main and some other stuff in one
void loop()
{
    //execute whatever is coming in on the serial port
    execute(readSerial());
    
    //ask for next instruction
    confirmExecution();
}

//reads incoming line of code off serial port
String readSerial()
{
    String input;

    while(input == "\n")
    {
        input = Serial.readString();
    }
    return input;
}

//tells serial port that command has been executed and to send next command
void confirmExecution()
{
    Serial.print("executed");
}

//takes a single line of the instruction code from the control computer and executes action
//instructions format: "identifier input1 input2"
void execute (String instructions)
{
    //check what the identifier is (each identfiers handler will know how many inputs it needs and will check the rest)
    int space1 = instructions.indexOf(" ");
    String identifier = instructions.substring(0, space1);

    //toggle pen 
    //("p", "down" = put it down, "up" = bring it up)
    if(identifier = "p")
    {
        int space2 = instructions.indexOf(" ", space1);
        String direction = identifier.substring(space2);

        //check if pen is where it is supposed to be and if it is then do nothing
        if((direction == "down" and grid.penIsDown) or (direction == "up" and grid.penIsDown == false))
        {
            return;
        }

        grid.togglePen();
    }

    //move to point
    //("m", "int" xCord, "int" yCord )
    if(identifier = "m")
    {
        int space2 = instructions.indexOf(" ", space1);
        int space3 = instructions.indexOf(" ", space2);

        int xTarget = instructions.substring(space2, space3).toInt();
        int yTarget = instructions.substring(space3).toInt();

        grid.move(xTarget, yTarget);
    }

    //zero axis
    //("z")
    if(identifier = "z")
    {
        grid.zeroAxis();
    }

    //jog x Axis
    //("jx")
    if(identifier = "jx")
    {
        int space2 = instructions.indexOf(" ", space1);
        double distance = instructions.substring(space2).toDouble();

        grid.runXBySegment(distance);
    }

    //jog y Axis
    //("jy")
    if(identifier = "jy")
    {
        int space2 = instructions.indexOf(" ", space1);
        double distance = instructions.substring(space2).toDouble();

        grid.runYBySegment(distance);
    }
}