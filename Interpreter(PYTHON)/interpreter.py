class Interpreter:
    def __init__(self, grid):
        self.grid = grid
        self.variables = {}
        self.commands = {
            'SET': self.set_color,
            'FOR': self.for_loop,
            'IF': self.if_statement,
            'PRINT': self.print_message,
            'INT': self.declare_int,
            'FLOAT': self.declare_float,
            'STR': self.declare_str,
            'MATRIX': self.declare_matrix,
            'VECTOR': self.declare_vector,
            'INPUT': self.input_var,
            'WHILE': self.while_loop
        }

    def execute_script(self, script_path):
        with open(script_path, 'r') as file:
            script = file.read()
        self.execute_commands(script)

    def execute_commands(self, commands):
        lines = commands.splitlines()
        i = 0
        while i < len(lines):
            line = lines[i].strip()
            if not line or line.startswith('#'):
                i += 1
                continue
            parts = line.split()
            command = parts[0].upper()
            args = parts[1:]
            if command in self.commands:
                self.commands[command](args)
            i += 1

    def set_color(self, args):
        x, y, color = int(self.evaluate(args[0])), int(self.evaluate(args[1])), args[2].lower()
        colors = {
            'red': (255, 0, 0),
            'green': (0, 255, 0),
            'blue': (0, 0, 255),
            'black': (0, 0, 0),
            'white': (255, 255, 255)
        }
        if color in colors:
            self.grid.set_color(x, y, colors[color])

    def for_loop(self, args):
        x1, x2, y1, y2 = map(int, map(self.evaluate, args[:4]))
        block_commands = ' '.join(args[4:])[1:-1].strip()
        for y in range(y1, y2 + 1):
            for x in range(x1, x2 + 1):
                self.execute_commands(block_commands)

    def if_statement(self, args):
        condition = args[0].lower()
        block_commands = ' '.join(args[1:])[1:-1].strip()
        if self.evaluate_condition(condition):
            self.execute_commands(block_commands)

    def while_loop(self, args):
        condition = args[0].lower()
        block_commands = ' '.join(args[1:])[1:-1].strip()
        while self.evaluate_condition(condition):
            self.execute_commands(block_commands)

    def declare_int(self, args):
        self.variables[args[0]] = int(self.evaluate(args[1]))

    def declare_float(self, args):
        self.variables[args[0]] = float(self.evaluate(args[1]))

    def declare_str(self, args):
        self.variables[args[0]] = self.evaluate(args[1])

    def declare_matrix(self, args):
        name = args[0]
        rows, cols = int(args[1]), int(args[2])
        self.variables[name] = [[0] * cols for _ in range(rows)]

    def declare_vector(self, args):
        name = args[0]
        size = int(args[1])
        self.variables[name] = [0] * size

    def input_var(self, args):
        var_name = args[0]
        value = input(f"Enter value for {var_name}: ")
        self.variables[var_name] = self.evaluate(value)

    def print_message(self, args):
        message = ' '.join(args)
        print(self.evaluate(message))

    def evaluate(self, expression):
        if expression.isdigit():
            return int(expression)
        try:
            return float(expression)
        except ValueError:
            pass
        if expression in self.variables:
            return self.variables[expression]
        return expression.strip('"')

    def evaluate_condition(self, condition):
        # Simplificação: qualquer condição "true" será verdadeira
        return condition == 'true'  # Deve ser melhorado para avaliar expressões reais
