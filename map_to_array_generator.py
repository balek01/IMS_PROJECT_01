from PIL import Image
import numpy as np
from collections import OrderedDict


# Function to compute the mean color value of a given image region
def compute_mean_color(image, start_x, start_y, width, height):
    region = image.crop((start_x, start_y, start_x + width, start_y + height))
    pixels = np.array(region)
    mean_color = np.mean(pixels, axis=(0, 1))[:3]
    return mean_color.astype(int) 


# Function to find the closest color from a list
def find_closest_color(target_color, color_list):
  # Reshape target_color to (1, 3)
    target_color = target_color.reshape(1, -1)
    
    # Compute color distances
    color_distances = np.linalg.norm(color_list - target_color, axis=1)
    
    # Find the index of the closest color
    closest_color_index = np.argmin(color_distances)
    
    return color_list[closest_color_index]


# Sample image file path
image_path = "hradec.png"
region_size =5
# Read the image
img = Image.open(image_path)

# Define the regions and colors
regions = [(i, j) for i in range(0, img.width, region_size) for j in range(0, img.height, region_size)]
colors = np.array(
    [
        # road
        [155, 214, 114],
        [250, 239, 117],
        [111, 161, 87],
        [254, 254, 254],
        [255, 203, 119],
        # city
        [226, 213, 183],
        # land
        [241, 240, 229],
        [154, 191, 220],
        # forest
        [207, 229, 179],
    ]
)

# Compute mean color for each region, find the closest color, and store the results
results = {}
for region in regions:

    mean_color = compute_mean_color(img, region[0], region[1], region_size, region_size)
    closest_color = find_closest_color(mean_color, colors)
    results[region] = closest_color


# Sort the results by color
sorted_results = OrderedDict(sorted(results.items(), key=lambda x: tuple(x[1])))

# Determine the size of the result_array based on the maximum indices
max_row = max(region[1] // region_size for region in sorted_results)
max_col = max(region[0] // region_size for region in sorted_results)
result_array = np.full((max_row + 1, max_col + 1), "", dtype="<U1")

# Populate the result_array with letters 'R', 'B', and 'O' based on sorted colors
for region, color in sorted_results.items():
    row, col = region[1] // region_size, region[0] // region_size
    letter = (
        "R"
        if np.array_equal(color, [155, 214, 114]) or np.array_equal(color, [250, 239, 117]) 
        or np.array_equal(color, [111, 161, 87]) or np.array_equal(color, [155, 214, 114]) 
        or np.array_equal(color, [254, 254, 254]) or np.array_equal(color, [255, 203, 119])
        else "C"
        if np.array_equal(color, [226, 213, 183])
        else "F"
        if  np.array_equal(color, [207, 229, 179])
        else "L"
    )
    result_array[row, col] = letter

# Print the 2D array
print("{")
for row in result_array:
    print("{'")
    print("', '".join(row))
    print("'},")
print("}")
