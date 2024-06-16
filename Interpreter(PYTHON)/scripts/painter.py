import pygame
import numpy as np

# Initialize Pygame
pygame.init()

# Define colors corresponding to your Arduino colors array
colors = {
    'black': (0, 0, 0),
    'blue': (0, 0, 255),
    'green': (0, 255, 0),
    'cyan': (0, 255, 255),
    'red': (255, 0, 0),
    'magenta': (255, 0, 255),
    'yellow': (255, 255, 0),
    'white': (255, 255, 255)
}

color_keys = list(colors.keys())
current_color_index = 4  # Start with red
current_color = color_keys[current_color_index]

# Grid settings
grid_size = 25
pixel_size = 20
width, height = grid_size * pixel_size, grid_size * pixel_size

# Create the display window
window = pygame.display.set_mode((width, height))
pygame.display.set_caption('Pygame Grid Drawing')

# Create the grid
grid = np.zeros((grid_size, grid_size), dtype=object)

def draw_grid():
    for i in range(grid_size):
        for j in range(grid_size):
            color = colors[grid[i, j]] if grid[i, j] else colors['white']
            pygame.draw.rect(window, color, (i * pixel_size, j * pixel_size, pixel_size, pixel_size))
            pygame.draw.rect(window, (0, 0, 0), (i * pixel_size, j * pixel_size, pixel_size, pixel_size), 1)

def get_cell(x, y):
    return x // pixel_size, y // pixel_size

def generate_set_commands():
    arduino_colors = {
        'black': 0,
        'blue': 1,
        'green': 2,
        'cyan': 3,
        'red': 4,
        'magenta': 5,
        'yellow': 6,
        'white': 7
    }
    
    commands = []
    for i in range(grid_size):
        for j in range(grid_size):
            if grid[i, j]:
                color_value = arduino_colors[grid[i, j]]
                commands.append(f"SET {i*10} {j*10} 10 {color_value}")
    return commands

running = True
press = False
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        
        if event.type == pygame.MOUSEBUTTONDOWN:
            press=True
        elif event.type == pygame.MOUSEBUTTONUP:
            press=False
        if press:
            x, y = event.pos
            i, j = get_cell(x, y)
            if i >=0 and i<grid_size and j >=0 and j<grid_size:
                grid[i, j] = current_color
        
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_c:
                current_color_index = (current_color_index + 1) % len(color_keys)
                current_color = color_keys[current_color_index]
                print(f"Current color changed to: {current_color}")
            if event.key == pygame.K_g:
                print("Generated SET Commands:")
                commands = generate_set_commands()
                for cmd in commands:
                    print(cmd)

    window.fill((255, 255, 255))
    draw_grid()
    pygame.display.flip()

pygame.quit()
