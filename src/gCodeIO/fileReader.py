#Reads code from the gCode file and sends on serial port to arduino
import time
import serial

print("\n\n\n")

filePath = r"C:\\Users\\manav\\Documents\\PlatformIO\\Projects\\Plotter\\src\\gCodeIO\\file.txt"

#open serial port
serialRead = serial.Serial(port = '\\\\.\\COM8', baudrate = 9600, bytesize = 8, timeout = 2, stopbits=serial.STOPBITS_ONE)

#open file
code = open(filePath, "r")

waitTime = 0.01

#reads whats on serial port, prints it, returns it
def readSerial():
    current = serialRead.readline().decode('latin-1')
    print("serial read: " + str(current))
    return current

#writes what is told to to serial port, prints it
def writeToSerial(inp):
    print("serial write: " + inp)
    serialRead.write(str.encode(inp))
    
#wait for user to say go
def startLoop():
    #will block anything else until user enters something prompting program to start
    x = input("enter something to start")
    time.sleep(5)
    writeToSerial("y")
        
#wait for machine to be ready for command
def waitForGo():
    go = False
    while(1==1):
        time.sleep(waitTime)
        x = readSerial()
        if("go" in x):
            return
    
"""
    while(go == False):
        time.sleep(waitTime)
        serialRead.readline()
        if(readSerial().__eq__("go") == 0):
            go = True
            print("weeeeee!")
"""
            
#run incoming commands
def run():
    line = code.readline()
    while(line != ""):
        waitForGo()
        writeToSerial(line)
        line = code.readline()

def printStream(times):
    a = 0
    while(a < times):
        readSerial()
        time.sleep(waitTime)
        a+=1 

startLoop()
run()


"""
a = 0   
while(a < 5):
    time.sleep(1)
    x = serialRead.readline().decode('latin-1')
    if(len(x) < 13):
        print(x)
    if(x.__eq__("hello") == 0):
       print("hi")
    a+=1 
"""