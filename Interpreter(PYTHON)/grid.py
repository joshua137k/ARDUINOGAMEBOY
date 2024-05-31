class Grid:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.grid = [[(255, 255, 255) for _ in range(width)] for _ in range(height)]

    def set_color(self, x, y, color):
        if 0 <= x < self.width and 0 <= y < self.height:
            self.grid[y][x] = color

    def get_color(self, x, y):
        if 0 <= x < self.width and 0 <= y < self.height:
            return self.grid[y][x]
        return (255, 255, 255)
