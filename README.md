# Plotter
## A code base to control custom 2-axis drawing machines
### Purpose:
#### Use the programs in this code base to control any Arduino-controlled, stepper motor based, 2.5-axis drawing machine
#### More details about the hardware in my particular design, including parts used and a link to the onshape document can be found here: 
### How to use:
#### Overview - This codebase works on the idea of splitting machine up into 10 even 'segments' in the x and y directions that take up the entirety of the machine's work area. machineControl.cpp runs on the arduino which controls the machine, and communicates with an external computer over serial port. The external machine can use the slicer programs--primarily integratedSlicer.py--to turn a jpg image into a txt-document based code that can be communicated to the machine over serial port. It can use fileReader.py in the gCodeIO folder to execute the code stored in the txt document, which sends lines in the txt document over serial to the arduino to interpret.
