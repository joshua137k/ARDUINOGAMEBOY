import random
import matplotlib.pyplot as plt

# Define o tamanho do labirinto
WIDTH = 10
HEIGHT = 10

# Define as direções possíveis (cima, baixo, esquerda, direita)
DIRECTIONS = [(0, 1), (1, 0), (0, -1), (-1, 0)]

def is_valid_move(maze, x, y):
    """Verifica se a célula pode ser visitada"""
    if x < 0 or x >= WIDTH or y < 0 or y >= HEIGHT:
        return False
    if maze[x][y] == 1:
        return False
    return True

def generate_maze(maze, x, y):
    """Gera o labirinto usando DFS"""
    maze[x][y] = 1
    random.shuffle(DIRECTIONS)
    for dx, dy in DIRECTIONS:
        nx, ny = x + dx * 2, y + dy * 2
        if is_valid_move(maze, nx, ny):
            maze[x + dx][y + dy] = 1
            generate_maze(maze, nx, ny)

def create_maze():
    """Cria o labirinto inicial"""
    maze = [[0 for _ in range(HEIGHT)] for _ in range(WIDTH)]
    generate_maze(maze, 0, 0)
    return maze

def plot_maze(maze):
    """Desenha o labirinto"""
    plt.figure(figsize=(10, 10))
    plt.imshow(maze, cmap='binary')
    plt.xticks([])
    plt.yticks([])
    plt.show()

# Gera e desenha o labirinto
maze = create_maze()
f = open("Interpreter(PYTHON)\scripts\labirint.txt","w")
for x,xv in enumerate(maze):
    for y,yv in enumerate(xv):
        if (maze[x][y]==1):
            f.write(f"SET {(x+1)*20} {(y+1)*20} size 0\n")
f.close()
plot_maze(maze)
