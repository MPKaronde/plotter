import cv2
import numpy as np
from PIL import Image

def process_image(image_path, resolution, grid_size=10, epsilon_factor=0.01):
    # Load image
    img = Image.open(image_path).convert('L')  # Convert to grayscale
    img_array = np.array(img)

    # Rotate the image by 180 degrees
    img_array = cv2.rotate(img_array, cv2.ROTATE_180)

    # Convert to pure black and white
    _, bw_img = cv2.threshold(img_array, 128, 255, cv2.THRESH_BINARY)

    # Apply Gaussian blur to reduce noise
    blurred_img = cv2.GaussianBlur(bw_img, (5, 5), 0)

    # Use adaptive thresholding for better contour detection
    binary_img = cv2.adaptiveThreshold(blurred_img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                       cv2.THRESH_BINARY_INV, 11, 2)

    # Find contours
    contours, _ = cv2.findContours(binary_img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Get image dimensions
    height, width = binary_img.shape

    # Normalize coordinates
    def normalize_coord(x, y):
        return (round(x * (grid_size - 1) / width, 3), round(y * (grid_size - 1) / height, 3))

    # Determine epsilon dynamically based on resolution
    epsilon = epsilon_factor * resolution

    # Extract and format coordinates for each contour
    output = ""
    coordinates_list = []
    pen_down = False

    for contour in contours:
        # Approximate the contour to remove points on straight lines
        approx_contour = cv2.approxPolyDP(contour, epsilon * cv2.arcLength(contour, True), True)

        # Remove extra dimension and normalize
        contour_coords = []
        for point in approx_contour[:, 0, :]:
            nx, ny = normalize_coord(point[0], point[1])
            contour_coords.append((nx, ny))

        if len(contour_coords) < 2:
            continue  # Skip contours that are too short to draw

        # Ensure that closed contours are properly closed
        if contour_coords[0] != contour_coords[-1]:
            contour_coords.append(contour_coords[0])

        # Determine if a penDown command is needed
        if not pen_down:
            output += "pd\n"
            pen_down = True

        # Append the contour points with resolution-based spacing
        last_point = None
        for i, coord in enumerate(contour_coords):
            if last_point:
                # Calculate distance between last point and current point
                dist = np.linalg.norm(np.array(coord) - np.array(last_point))
                # Insert intermediate points if distance exceeds resolution threshold
                while dist > resolution:
                    # Calculate intermediate point
                    ratio = resolution / dist
                    intermediate_x = last_point[0] + ratio * (coord[0] - last_point[0])
                    intermediate_y = last_point[1] + ratio * (coord[1] - last_point[1])
                    output += f"rtp {round(intermediate_x, 3)} {round(intermediate_y, 3)}\n"
                    last_point = (intermediate_x, intermediate_y)
                    dist = np.linalg.norm(np.array(coord) - np.array(last_point))
            output += f"rtp {coord[0]} {coord[1]}\n"
            last_point = coord

        # End of the current contour
        output += "pu\n"
        pen_down = False

        # Store the contour coordinates for connection checking
        coordinates_list.append(contour_coords)

    # Ensure final pen up command if needed
    if pen_down:
        output += "pu\n"

    output += "end"
    return output
