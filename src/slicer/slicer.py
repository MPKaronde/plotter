import cv2
import numpy as np
from PIL import Image

def process_image(image_path, grid_size=10):
    # Load image
    img = Image.open(image_path).convert('L')  # Convert to grayscale
    img_array = np.array(img)

    # Threshold the image to binary
    _, binary_img = cv2.threshold(img_array, 128, 255, cv2.THRESH_BINARY_INV)

    # Find contours
    contours, _ = cv2.findContours(binary_img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Get image dimensions
    height, width = binary_img.shape

    # Normalize coordinates
    def normalize_coord(x, y):
        return (round(x * (grid_size - 1) / width, 3), round(y * (grid_size - 1) / height, 3))

    # Extract and sort coordinates
    coordinates = []
    for contour in contours:
        # Extract points and sort them in a clockwise order
        contour = contour[:, 0, :]
        contour = [tuple(point) for point in contour]
        
        # Compute centroid
        M = cv2.moments(np.array(contour, dtype=np.int32))
        cx = int(M['m10'] / M['m00'])
        cy = int(M['m01'] / M['m00'])
        
        # Sort points based on angle around the centroid
        sorted_contour = sorted(contour, key=lambda p: np.arctan2(p[1] - cy, p[0] - cx))

        # Normalize and add to list
        for x, y in sorted_contour:
            nx, ny = normalize_coord(x, y)
            coordinates.append((nx, ny))
    
    # Ensure the shape is closed by repeating the first point at the end
    if coordinates:
        coordinates.append(coordinates[0])

    # Format coordinates as a string
    def format_coordinates(coords):
        return '\n'.join(f"{x:.3f} {y:.3f}" for x, y in coords)

    return format_coordinates(coordinates)

#format to lines that can be run
def formatToRun(coords):
    writePath = r"C:\\Users\\manav\\Documents\\PlatformIO\\Projects\\Plotter\\src\\gCodeIO\\file.txt"
    code = open(writePath, "+w")
    cordList = coords.split("\n")
    for a in cordList:
        out = "rtp "
        out += a[:1] + " "
        out += a[2:] + "\n"
        code.write(out)
    code.write("end")

def main():
    image_path = 'C:\\Users\\manav\\Documents\\PlatformIO\\Projects\\Plotter\\src\\slicer\\vectors\\test3.png'  # Replace with your image file path
    formatted_coordinates = process_image(image_path)
    
    print("2D coordinates on the grid:")
    print(formatted_coordinates)
    formatToRun(formatted_coordinates)
    
if __name__ == "__main__":
    main()
