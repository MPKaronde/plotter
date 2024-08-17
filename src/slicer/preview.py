import matplotlib.pyplot as plt
import numpy as np

filePath = r"C:\\Users\\manav\\Documents\\PlatformIO\\Projects\\Plotter\\src\\gCodeIO\\file.txt"

#read from file and format to np.array
def readFromFile(path):
    penDown = False
    ret = []
    code = open(path, '+r')
    line = code.readline()
    while(line != "end"):
        #check if line is an rtp line and pen is down
        if line.find("rtp") != -1 and penDown:
            space1 = line.find(" ")
            space2 = line.find(" ", space1 + 1)
            x = line[space1:space2].strip()
            y = line[space2:].strip()
            x = float(x)
            y = float(y)
            ret.append([x, y])
        #penUp/Down
        if line.find("pu") != -1:
            penDown = False
        if line.find("pd") != -1:
            penDown = True
        line = code.readline()
    return(np.array(ret))
            
#draws the coordinates
def plot_lines_on_grid(coords):
    # Ensure the input is a numpy array
    coords = np.array(coords)

    # Check if the input is a 2D array with two columns
    if coords.ndim != 2 or coords.shape[1] != 2:
        raise ValueError("Input should be a 2D numpy array with two columns for x and y coordinates.")

    # Extract x and y coordinates
    x = coords[:, 0]
    y = coords[:, 1]

    # Create a plot
    plt.figure(figsize=(6, 6))
    plt.plot(x, y, marker='o', linestyle='-', color='b')  # Line with markers
    plt.xlim(0, 10)  # Set x-axis limit
    plt.ylim(0, 10)  # Set y-axis limit
    plt.grid(True)   # Show grid

    # Add labels and title
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.title('Line Plot on a 10x10 Grid')

    # Display the plot
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()
