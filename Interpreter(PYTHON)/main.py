import pygame
from grid import Grid
from interpreter import Interpreter
import os
import threading



class Button:
    def __init__(self, x, y, width, height, color, text):
        self.rect = pygame.Rect(x, y, width, height)
        self.color = color
        self.text = text
        self.font = pygame.font.Font(None, 36)
        self.text_surf = self.font.render(text, True, (255, 255, 255))

    def draw(self, screen):
        pygame.draw.rect(screen, self.color, self.rect)
        text_rect = self.text_surf.get_rect(center=self.rect.center)
        screen.blit(self.text_surf, text_rect)

    def is_clicked(self, pos):
        return self.rect.collidepoint(pos)


class GameEngine:
    def __init__(self):
        pygame.init()
        self.width = 600
        self.height = 600
        self.cell_size = 20
        self.grid = Grid(300, 300)
        
        self.colors = {
            'red': (255, 0, 0),
            'green': (0, 255, 0),
            'blue': (0, 0, 255),
            'black': (0, 0, 0),
            'white': (255, 255, 255)
        }
        self.buttons = [
            Button(10, 500, 100, 50, self.colors['red'], "LEFT"),
            Button(130, 500, 100, 50, self.colors['red'], "RIGHT"),
            Button(250, 500, 100, 50, self.colors['red'], "UP"),
            Button(370, 500, 100, 50, self.colors['red'], "DOWN")
        ]
        self.btnsClick = {
            "LEFT": False,
            "RIGHT": False,
            "UP": False,
            "DOWN": False
        }
        self.interpreter = Interpreter(self.grid,self.btnsClick)

    def run(self, script_name):
        
        screen = pygame.display.set_mode((self.width, self.height))
        pygame.display.set_caption('2D Game Engine')

        script_path = os.path.join(os.path.join("Interpreter(PYTHON)",'scripts'), script_name)
        #self.interpreter.execute_script(script_path)
        interpreter_thread = threading.Thread(target=self.interpreter.execute_script, args=(script_path,))
        interpreter_thread.start()

        running = True
        
        while running:
            
            
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    pos = pygame.mouse.get_pos()
                    for button in self.buttons:
                        self.btnsClick[button.text]=button.is_clicked(pos)
                elif event.type == pygame.MOUSEBUTTONUP:
                    for button in self.btnsClick:
                        self.btnsClick[button]=False


            self.interpreter.buttons = self.btnsClick
            screen.fill(self.colors['white'])
            self.draw_grid(screen)
            self.draw_buttons(screen)
            pygame.display.flip()
            

        pygame.quit()
        interpreter_thread.join()

    def draw_grid(self, screen):
        for y in range(300):
            for x in range(300):
                color = self.grid.get_color(x, y)
                pygame.draw.rect(screen, color, (x * self.cell_size, y * self.cell_size, self.cell_size, self.cell_size))

    def draw_buttons(self, screen):
        for button in self.buttons:
            button.draw(screen)



if __name__ == "__main__":
    engine = GameEngine()
    engine.run('mazeGame.txt')  # Você pode alterar o nome do script aqui
