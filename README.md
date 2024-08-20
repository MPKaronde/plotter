# Plotter
## A code base to control custom 2-axis drawing machines

### Purpose:
#### Use the programs in this code base to control any Arduino-controlled, stepper motor based, 2.5-axis drawing machine
#### More details about the hardware in my particular design, including parts used and a link to the onshape document can be found here: 

### How to use:

#### Overview
##### This codebase works on the idea of splitting machine up into 10 even 'segments' in the x and y directions that take up the entirety of the machine's work area. machineControl.cpp runs on the arduino which controls the machine, and communicates with an external computer over serial port. The external machine can use the slicer programs--primarily integratedSlicer.py--to turn a jpg image into a txt-document based code that can be communicated to the machine over serial port. It can use fileReader.py in the gCodeIO folder to execute the code stored in the txt document, which sends lines in the txt document over serial to the arduino to interpret.

#### machineControl.cpp
##### This is the file that is run on the arduino. Note that everything in this file to do with the y-axis assumes two y-motors that must act in opposite directions. Before loading it there are certain parameters that need to be attained and updated, most of which are self explanatory, but some of which may require guidance:
  * ##### The pin numbers for all your stepper motors. Note: for Arduino Uno and Nano (and potentially others), some analog ports will have to be used as digital ports for this purpose. This can be achieved by simply plugging the steppers into the analog ports and listing them as a continuation of the digital ports in the code.
  * ##### Axis speeds / acceleration: the rates at which you're x and y axis's will move
  * ##### Z-motor parameters: the rates at which you're z-motor will move
    * ###### zLiftDistance: the distance you're z motor must move to go from all the way up to pen down.
  * ##### Translation parameters: Currently best found by trial and error. Note: both assume you use the same motors and translate motion in the same way for both axis's. If this is not the case you will have to make a seperate one for each axis and implement it accordingly.
    * ###### stepsPerSegment: the number of motor steps the x / y axis motors must rotate to move a tenth of the machine's work area.
    * ###### segmentsPerCm: the number of segments in a Cm (or more likely, 1 over the numer of Cm in a segment)



