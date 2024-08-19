#draws a preview of given file to see before sending to run
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation

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
  
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Slider

def plot_lines_on_grid(coords):
    # Ensure the input is a numpy array
    coords = np.array(coords)

    # Check if the input is a 2D array with two columns
    if coords.ndim != 2 or coords.shape[1] != 2:
        raise ValueError("Input should be a 2D numpy array with two columns for x and y coordinates.")

    # Create a plot
    fig, ax = plt.subplots(figsize=(6, 6))
    plt.subplots_adjust(left=0.1, bottom=0.25)  # Adjust to make room for the slider
    ax.set_xlim(0, 10)  # Set x-axis limit
    ax.set_ylim(0, 10)  # Set y-axis limit
    ax.grid(True)       # Show grid

    # Initialize lists to store the coordinates of the drawn lines and transversals
    draw_xdata, draw_ydata = [], []
    trans_xdata, trans_ydata = [], []

    # Prepare the data by identifying when to draw and when to make a transversal
    data = []
    for i in range(1, len(coords)):
        segment = np.array([coords[i-1], coords[i]])
        if np.linalg.norm(coords[i] - coords[i-1]) >= 1.0:  # Threshold distance to detect transversal
            data.append((segment, 'transversal'))
        else:
            data.append((segment, 'draw'))

    # Initialize slider axis and slider
    slider_ax = plt.axes([0.1, 0.1, 0.65, 0.03], facecolor='lightgoldenrodyellow')
    slider = Slider(slider_ax, 'Frame', 0, len(data)-1, valinit=0, valstep=1)

    def update(val):
        frame = int(slider.val)
        ax.clear()
        ax.set_xlim(0, 10)
        ax.set_ylim(0, 10)
        ax.grid(True)
        draw_xdata, draw_ydata = [], []
        trans_xdata, trans_ydata = [], []
        
        for i in range(frame + 1):
            segment, segment_type = data[i]
            if segment_type == 'draw':
                draw_xdata.extend(segment[:, 0])
                draw_ydata.extend(segment[:, 1])
                ax.plot(draw_xdata[-2:], draw_ydata[-2:], marker='o', linestyle='-', color='b')
            elif segment_type == 'transversal':
                trans_xdata.extend(segment[:, 0])
                trans_ydata.extend(segment[:, 1])
                ax.plot(trans_xdata[-2:], trans_ydata[-2:], linestyle='--', color='r')

        # Update labels and title
        ax.set_xlabel('X Coordinate')
        ax.set_ylabel('Y Coordinate')
        ax.set_title('Line Plot on a 10x10 Grid')
        ax.legend(['Pen Down', 'Transversal'])

    # Attach update function to slider
    slider.on_changed(update)

    # Initial update
    update(0)

    # Show plot
    plt.show()

# Example usage:
# coords = readFromFile(filePath)
# plot_lines_on_grid(coords)
