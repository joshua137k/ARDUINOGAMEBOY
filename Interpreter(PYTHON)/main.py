import pygame
from grid import Grid
from interpreter import Interpreter
import os

class GameEngine:
    def __init__(self):
        self.width = 600
        self.height = 600
        self.cell_size = 10
        self.grid = Grid(300, 300)
        self.interpreter = Interpreter(self.grid)
        self.colors = {
            'red': (255, 0, 0),
            'green': (0, 255, 0),
            'blue': (0, 0, 255),
            'black': (0, 0, 0),
            'white': (255, 255, 255)
        }

    def run(self, script_name):
        pygame.init()
        screen = pygame.display.set_mode((self.width, self.height))
        pygame.display.set_caption('2D Game Engine')
        
        script_path = os.path.join(os.path.join("Interpreter(PYTHON)",'scripts'), script_name)
        self.interpreter.execute_script(script_path)

        running = True
        
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False

            screen.fill(self.colors['white'])
            self.draw_grid(screen)
            pygame.display.flip()

        pygame.quit()

    def draw_grid(self, screen):
        for y in range(300):
            for x in range(300):
                color = self.grid.get_color(x, y)
                pygame.draw.rect(screen, color, (x * self.cell_size, y * self.cell_size, self.cell_size, self.cell_size))

if __name__ == "__main__":
    engine = GameEngine()
    engine.run('example_script2.txt')  # Você pode alterar o nome do script aqui
