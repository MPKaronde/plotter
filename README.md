# Plotter
## A code base to control custom 2-axis drawing machines

### **Purpose:**
#### Use the programs in this code base to control any Arduino-controlled, stepper motor based, 2.5-axis drawing machine
#### More details about the hardware in my particular design, including parts used and a link to the onshape document can be found here: 

### **How to use:**
#### **Overview**
##### This codebase works on the idea of splitting machine up into 10 even 'segments' in the x and y directions that take up the entirety of the machine's work area. machineControl.cpp runs on the arduino which controls the machine, and communicates with an external computer over serial port. The external machine can use the slicer programs--primarily integratedSlicer.py--to turn a jpg image into a txt-document based code that can be communicated to the machine over serial port. It can use fileReader.py in the gCodeIO folder to execute the code stored in the txt document, which sends lines in the txt document over serial to the arduino to interpret.
####
#### **machineControl.cpp**
##### This is the file that is run on the arduino. Note that everything in this file to do with the y-axis assumes two y-motors that must act in opposite directions. Before loading it there are certain parameters that need to be attained and updated, most of which are self explanatory, but some of which may require guidance:
  * ##### The pin numbers for all your stepper motors. Note: for Arduino Uno and Nano (and potentially others), some analog ports will have to be used as digital ports for this purpose. This can be achieved by simply plugging the steppers into the analog ports and listing them as a continuation of the digital ports in the code.
  * ##### Axis speeds / acceleration: the rates at which you're x and y axis's will move
  * ##### Z-motor parameters: the rates at which you're z-motor will move
    * ###### zLiftDistance: the distance you're z motor must move to go from all the way up to pen down.
  * ##### Translation parameters: Currently best found by trial and error. Note: both assume you use the same motors and translate motion in the same way for both axis's. If this is not the case you will have to make a seperate one for each axis and implement it accordingly.
    * ###### stepsPerSegment: the number of motor steps the x / y axis motors must rotate to move a tenth of the machine's work area.
    * ###### segmentsPerCm: the number of segments in a Cm (or more likely, 1 over the numer of Cm in a segment)
####   
#### **integratedSlicer.py**
##### This program will use the slicer2.py file and the preview.py file to translate an image into code for the machine and display it. There are only three paramaters in it that you may need to set:
  * ##### imagePath: the file path of the image you wish to slice
  * ##### writePath: the file path of the txt document you wish to write to
  * ##### resolution: how high fidelity you want the drawn image to be to the original. A lower number results in a higher resolution image, but at the expense of job time, and vice versa with a larger number. I have found 0.8 to be a good sweet-spot generally, but you can test it out by changing the parameter yourself and viewing the preview, which will reflect changes in resolution.
####   
#### **fileReader.py**
##### This program reads the code generated by integratedSlicer.py and sends it over serial to the arduino to run. It has two paraemeters which will require changing:
  * ##### filePath: the txt document from which to read code. Most likely the same as the writePath in integratedSlicer.py
  * ##### serial port: this is adjusted in the line just beneath filePath, in the 'port = ' field. If you're arduino is connected over USB, you will most likely only have to change the port number and nothing else. The port number can be found by reading the upload log when sending code to the arduino and looking for the phrase 'COM' followed by some number.
####   
#### **serialLibrary.txt** 
##### This is a library of all the functions that can be sent over serial communication to the arduino that it can understand. Note that due to an annoying bug, all single-phrase commands (commands with no parameters) manually typed over serial port, other than the initial 'would you like the program to begin y/n', must have a trailing space or they will not be registered by the machine. Also note that parameters are seperated by spaces, and have no backets, parenthesis, commas, or other punctuation associated with them.


