#Reads code from the 'gCode' file and sends on serial port to arduino
import time

#file path from which to read
filePath = "file.txt"

#open serial port
serial = open("\\.\COM1", "rw")

#open file
code = open(filePath, "r")

#wait until confirmation is given
def delay():
    while(1 == 1):
        if(serial.readline() == "executed"):
            return
        time.sleep(1)

#next line reader
line = code.readline()
while(line != ""):
    delay()
    serial.write(code.readline() + "\n")
    
        