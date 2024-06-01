class Interpreter:
    def __init__(self, grid,buttons):
        self.grid = grid
        self.buttons=buttons
        self.variables = {}
        self.functions = {}
        self.commands = {
            'SET': self.set_color,
            'IF': self.if_statement,
            'PRINT': self.print_message,
            'INT': self.declare_int,
            'FLOAT': self.declare_float,
            'STR': self.declare_str,
            'MATRIX': self.declare_matrix,
            'VECTOR': self.declare_vector,
            'INPUT': self.input_var,
            'WHILE': self.while_loop,
            'FUNC': self.define_function,
            'CALL': self.call_function,
            'BTNSTATE': self.btn_state,
            'GETMATRIXVALUE': self.get_matrix_value,
            'SETMATRIXVALUE': self.set_matrix_value

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
            if '=' in line:
                self.handle_assignment(line)
            else:
                parts = line.split()
                command = parts[0].upper()
                args = parts[1:]

                if command in ['WHILE', 'IF','FUNC']:
                    i = self.commands[command](args, i, lines)
                elif command in self.commands:
                    self.commands[command](args)
            i += 1

    def btn_state(self, args):
        button_name = args[0]
        var_name = args[1]
        self.variables[var_name] = self.buttons[button_name]

    def while_loop(self, args, i, lines):
        condition = ' '.join(args)
        block_commands = []
        nested_whiles = 0
        i += 1
        while i < len(lines):
            current_line = lines[i].strip()
            if current_line.startswith('WHILE'):
                nested_whiles += 1
            elif current_line.startswith('ENDWHILE'):
                if nested_whiles == 0:
                    break
                nested_whiles -= 1
            block_commands.append(lines[i])
            i += 1
        block_commands = '\n'.join(block_commands)
        while self.evaluate_condition(condition):
            self.execute_commands(block_commands)
        return i


    def handle_assignment(self, line):
        var_name, expression = map(str.strip, line.split('=', 1))

        self.variables[var_name] = self.evaluateMath(expression)


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


    def if_statement(self, args, i, lines):
        condition = ' '.join(args)
        block_commands = []
        nested_ifs = 0
        i += 1
        while i < len(lines):
            current_line = lines[i].strip()
            if current_line.startswith('IF'):
                nested_ifs += 1
            elif current_line.startswith('ENDIF'):
                if nested_ifs == 0:
                    break
                nested_ifs -= 1
            block_commands.append(lines[i])
            i += 1
        block_commands = '\n'.join(block_commands)
        if self.evaluate_condition(condition):
            self.execute_commands(block_commands)
        return i


    def declare_int(self, args):
        self.variables[args[0]] = int(self.evaluate(args[1]))
        

    def declare_float(self, args):
        self.variables[args[0]] = float(self.evaluate(args[1]))


    def declare_str(self, args):
        self.variables[args[0]] = self.evaluate(" ".join(args[1:]))


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
        message = ""
        is_str = False
        for i in args:
            if i.startswith('"') and i.endswith('"') and len(i)>1:
                message += i[1:-1] + " "
            elif len(i)==1 and  i=='"':
                is_str=False
                message+=" "
            elif i.startswith('"'):
                is_str = True
                message = i[1:] + " "
            elif i.endswith('"'):
                is_str = False
                message += i[:-1]+ " "
            elif is_str:
                message += i + " "
            else:
                message += str(self.evaluate(i)) + " "

        print(message.strip())


    def evaluateMath(self, expression):
        try:
            try:
                return eval(expression)
            except:
                return eval(expression, {}, self.variables)
        except Exception as e:
            print(f"Error evaluating expression '{expression}': {e}")
            return expression


    def evaluate(self, expression):
        if expression.isdigit():
            return int(expression)
        try:
            return float(expression)
        except ValueError:
            pass

        try:
            return eval(expression, {}, self.variables)
        except:
            return expression.strip('"')

    def get_matrix_value(self, args):
        matrix_name = args[0]
        row = int(self.evaluate(args[1]))
        col = int(self.evaluate(args[2]))
        var_name = args[3]
        if matrix_name in self.variables and isinstance(self.variables[matrix_name], list):
            self.variables[var_name] = self.variables[matrix_name][row][col]
        else:
            print(f"Error: Matrix '{matrix_name}' not found.")

    def set_matrix_value(self, args):
        matrix_name = args[0]
        row = int(self.evaluate(args[1]))
        col = int(self.evaluate(args[2]))
        value = self.evaluate(args[3])
        if matrix_name in self.variables and isinstance(self.variables[matrix_name], list):
            self.variables[matrix_name][row][col] = value
        else:
            print(f"Error: Matrix '{matrix_name}' not found.")

    def evaluate_condition(self, condition):
        try:
            condition = condition.replace("equals","==")
            condition = condition.replace("notE","!=")
            condition = condition.replace("AND","and")
            condition = condition.replace("OR","or")
            return eval(condition, {}, self.variables)
        except Exception as e:
            print(f"Error evaluating condition '{condition}': {e}")
            return False


    def define_function(self, args,i,lines):
        func_name = args[0]
        parameters = args[1:]
        block_commands = []
        i+=1
        while i < len(lines) and not lines[i].strip().startswith('ENDFUNC'):
            block_commands.append(lines[i])
            i += 1
        self.functions[func_name] = {'parameters': parameters, 'commands': '\n'.join(block_commands)}
        
        return i


    def call_function(self, args):
        func_name = args[0]
        arguments = args[1:]
        if func_name in self.functions:
            func = self.functions[func_name]
            local_vars = {}
            for param, arg in zip(func['parameters'], arguments):
                local_vars[param] = self.evaluate(arg)
            original_vars = self.variables.copy()
            self.variables.update(local_vars)
            self.execute_commands(func['commands'])
            self.variables = original_vars
        else:
            print(f"Function '{func_name}' not defined.")
