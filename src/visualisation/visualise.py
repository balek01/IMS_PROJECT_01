from PIL import Image, ImageDraw

def visualize_letters(filename):
    with open(filename, 'r') as file:
        rows = 200
        cols = 200

        # Create an image with a white background
        image = Image.new('RGB', (cols, rows), 'white')
        draw = ImageDraw.Draw(image)

        for i in range(rows):
            for j in range(cols):
                letter = file.read(1)
               
                   
                # Assign color based on the letter
                color = (0, 0, 0)  # Default to black for unknown letters
                if letter == 'F':
                    color = (0, 255, 0)  # greed
                elif letter == 'L':
                    color = (255, 255, 255)  # white
                elif letter == 'D':
                    color = (255, 0, 0)  # red
                elif letter == 'C':
                    color = (255, 255, 255)  # white
                elif letter == 'R':
                       color = (255, 255, 255)  # white
                

                # Draw a pixel with the assigned color
                draw.point((j, i), fill=color)

        # Save or display the image
        image.save('output.png')
        image.show()

if __name__ == "__main__":
    visualize_letters("output.txt")
