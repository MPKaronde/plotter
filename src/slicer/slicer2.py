import cv2
import numpy as np
from PIL import Image

def process_image(image_path, resolution, grid_size=10, epsilon=0.01):
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
    contours, _ = cv2.findContours(binary_img, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)

    # Get image dimensions
    height, width = binary_img.shape

    # Normalize coordinates
    def normalize_coord(x, y):
        return (round(x * (grid_size - 1) / width, 3), round(y * (grid_size - 1) / height, 3))

    # Extract and format coordinates for each contour
    coordinates_list = []
    for contour in contours:
        # Approximate the contour to remove points on straight lines
        approx_contour = cv2.approxPolyDP(contour, epsilon * cv2.arcLength(contour, True), True)

        # Remove extra dimension and normalize
        contour_coords = []
        for point in approx_contour[:, 0, :]:
            nx, ny = normalize_coord(point[0], point[1])
            contour_coords.append((nx, ny))

        # Check if the contour is closed by comparing the first and last points
        is_closed = np.array_equal(contour_coords[0], contour_coords[-1])

        # If closed, ensure the shape is closed by adding the first point at the end
        if is_closed and contour_coords:
            contour_coords.append(contour_coords[0])

        coordinates_list.append(contour_coords)

    # Format coordinates as a string
    def format_coordinates(coords_list):
        output = ""
        for coords in coords_list:
            if coords:
                output += "pd\n"
                for a in range(len(coords)):
                    if a % resolution == 0:
                        output += f"rtp {coords[a][0]} {coords[a][1]}\n"
                output += "pu\n"
        return output
    
    # Combine all formatted contours into one string
    output = format_coordinates(coordinates_list)
    output += "end"
    
    return output
