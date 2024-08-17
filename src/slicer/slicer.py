import cv2
import numpy as np
from PIL import Image

def process_image(image_path, resolution, grid_size=10):
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
    # currently only puts pen down at beggining and lifts it at end
    def format_coordinates(coords):
        out = "pd\n"
        for a in range(len(coords)):
            if a % resolution == 0:
                out += "rtp " + str(coords[a][0]) + " " + str(coords[a][1]) +  "\n"
        out += "pu\nend"
        return out
    
    return format_coordinates(coordinates)
