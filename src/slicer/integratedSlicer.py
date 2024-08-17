import cv2
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import slicer
import preview

#USER PARAMETERS
imagePath = 'C:\\Users\\manav\\Documents\\PlatformIO\\Projects\\Plotter\\src\\slicer\\vectors\\test3.png'
writePath = r"C:\\Users\\manav\\Documents\\PlatformIO\\Projects\\Plotter\\src\\gCodeIO\\file.txt"
resolution = 20

#get coordinates
cords = slicer.process_image(imagePath, resolution)

#write coordinates
code = open(writePath, "+w")
code.write(cords)
code.close()

#draw coordinates
preview.plot_lines_on_grid(preview.readFromFile(writePath))