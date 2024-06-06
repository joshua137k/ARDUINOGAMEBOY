#include <Arduino.h>
#include "Dictionary.h"
#include "Interpreter.h"


Dictionary variable(100);


char* strip(char* str) {
    while (isspace(*str)) str++;

    if (*str == 0)
        return str;

    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    *(end + 1) = '\0';

    return str;
}

int maxToken(char* str, char* delimit) {
    int count = 0;
    while (*str) {
        if (*str == *delimit) {
            count++;
        }
        str++;
    }
    return count + 1;
}

char** split(char* str, char* delimit, int* num_token) {
    char* str_copy = strdup(str);
    int token_count = maxToken(str, delimit);
    *num_token = token_count;
    char** parts = (char**)malloc(token_count * sizeof(char*));
    if (!parts) {
        free(str_copy);
        return NULL;
    }
    char* token = strtok(str_copy, delimit);
    int index = 0;
    while (token != NULL) {
        parts[index] = strdup(token);
        index++;
        token = strtok(NULL, delimit);
    }

    free(str_copy);
    return parts;
}

float evaluateMath(const char* expression) {
    // Suporta operações básicas: +, -, *, /
    char* expr_copy = strdup(expression);
    char* token = strtok(expr_copy, " ");
    float result = 0.0;
    char last_op = '+';
    while (token != NULL) {
        // Se o token é um operador, atualiza last_op
        if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            last_op = token[0];
        } else {
            // Se o token é um número ou uma variável, converte para float
            float value;
            ValueType type;
            if (variable.exists(token)) {
                Value var_value = variable.getValue(token, type);
                switch (type) {
                    case INT:
                        value = (float) var_value.intValue;
                        break;
                    case FLOAT:
                        value = var_value.floatValue;
                        break;
                }
            } else {
                value = atof(token);
            }
            switch (last_op) {
                case '+':
                    result += value;
                    break;
                case '-':
                    result -= value;
                    break;
                case '*':
                    result *= value;
                    break;
                case '/':
                    result /= value;
                    break;
            }
        }
        token = strtok(NULL, " ");
    }
    free(expr_copy);
    return result;
}

int btn_state(const char** args, int i, const char** lines,int partNumber,int num_tokens) {
    Serial.print("Button state with args: ");
    Serial.println(args[0]);  // Print the first argument
    return i++;
}

int while_loop(const char** args, int i, const char** lines,int partNumber,int num_tokens) {
    Serial.println("While loop with args: ");
    return i++;
}

void handle_assignment(const char* line) {
    // Divida a linha em nome da variável e expressão
    char* line_copy = strdup(line);
    char* var_name = strtok(line_copy, "=");
    char* expression = strtok(NULL, "=");
    
    if (var_name && expression) {
        var_name = strip(var_name);
        expression = strip(expression);
        
        // Avalie a expressão
        float value = evaluateMath(expression);
        ValueType type;
        Value v = variable.getValue(var_name, type); 
          switch(type) {
            case INT:
                v.intValue=(int)value;
                variable.change(var_name,v,type);
                break;
            case FLOAT:
                v.floatValue=value;
                variable.change(var_name,v,type);
                break;

        }
        
    }
    
    free(line_copy);
}

void set_color(const char** args,int partNumber) {
    Serial.print("Set color with args: ");
}

int if_statement(const char** args, int i, const char** lines,int partNumber,int num_tokens) {
    Serial.print("If statement with args: ");
    return i++;
}

void input_var(const char** args,int partNumber) {
    Serial.print("Input variable with args: ");
}

void print_message(const char** args, int partNumber) {
    String message = "";
    bool is_str = false;
    partNumber--;
    for (int i = 0; i < partNumber; i++) {
        String arg = String(args[i]);
        
        if (arg.startsWith("'") && arg.endsWith("'") && arg.length() > 1) {
            message += arg.substring(1, arg.length() - 1) + " ";
        } else if (arg.length() == 1 && arg == "'") {
            is_str = false;
            message += " ";
        } else if (arg.startsWith("'")) {
            is_str = true;
            message = arg.substring(1) + " ";
        } else if (arg.endsWith("'")) {
            is_str = false;
            message += arg.substring(0, arg.length() - 1) + " ";
        } else if (is_str) {
            message += arg + " ";
        } else {
            ValueType type;
            Value value = variable.getValue(arg.c_str(), type); 
            message += variable.valueToString(value, type);
        }
    }

    Serial.println(message);
}

void get_matrix_value(const char** args,int partNumber) {
    Serial.print("Get matrix value with args: ");
}

void set_matrix_value(const char** args,int partNumber) {
    Serial.print("Set matrix value with args: ");
}

int define_function(const char** args, int i, const char** lines,int partNumber,int num_tokens) {
    Serial.print("Define function with args: ");
    return i++;
}

void call_function(const char** args,int partNumber) {
    Serial.print("Call function with args: ");
}

void call_BTNfunction(const char** args,int partNumber) {
    Serial.print("Call button function with args: ");
}

void declare_int(const char** args,int partNumber) {

    variable.addItem(args[0], atoi(args[1]));

}

void declare_float(const char** args,int partNumber) {
    variable.addItem(args[0], (float)atof(args[1]));

}

void declare_str(const char** args,int partNumber) {
    variable.addItem(args[0], atoi(args[1]));

}

void declare_matrix(const char** args,int partNumber) {
    int rows = atoi(args[1]);
    int cols = atoi(args[2]);

    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    variable.addItem(args[0], matrix);

}

void declare_vector(const char** args,int partNumber) {
    int* i =new int[atoi(args[1])];
    variable.addItem(args[0], &i);

}

void evaluate(const char** expression,int partNumber) {

    Serial.print("Evaluate expression: ");
}

void evaluate_condition(const char** condition,int partNumber) {
    Serial.print("Evaluate condition: ");
}




const char* commandNames[] = {
    "SET",
    "INPUT",
    "PRINT",
    "GETMATRIXVALUE",
    "SETMATRIXVALUE",
    "CALL",
    "INT",
    "FLOAT",
    "STR",
    "MATRIX",
    "VECTOR"
};

Command commands[] = {
    {"FUNC_BTNSTATE", btn_state, NULL},
    {"WHILE", while_loop, NULL},
    {"SET", NULL, set_color},
    {"IF", if_statement, NULL},
    {"INPUT", NULL, input_var},
    {"PRINT", NULL, print_message},
    {"GETMATRIXVALUE", NULL, get_matrix_value},
    {"SETMATRIXVALUE", NULL, set_matrix_value},
    {"FUNC", define_function, NULL},
    {"CALL", NULL, call_function},
    {"INT", NULL, declare_int},
    {"FLOAT", NULL, declare_float},
    {"STR", NULL, declare_str},
    {"MATRIX", NULL, declare_matrix},
    {"VECTOR", NULL, declare_vector}
};





int callFunctionByNameINT(const char* command_name, const char** args, int i, const char** lines,int partNumber,int num_tokens) {
    for (int j = 0; j < sizeof(commands) / sizeof(Command); j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            if (commands[j].runFuncInt != NULL) {
                 
                return commands[j].runFuncInt(args, i, lines, partNumber, num_tokens);
            } else {
                printf("Função com três parâmetros não encontrada para o comando: %s\n", command_name);
            }
        }
    }
    printf("Comando não encontrado: %s\n", command_name);
    return -1;
}

void callFunctionByName(const char* command_name, const char** args, int partNumber) {
    for (int j = 0; j < sizeof(commands) / sizeof(Command); j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            if (commands[j].runFuncVoid != NULL) {
                
                commands[j].runFuncVoid(args,partNumber);
            } else {
                printf("Função com um parâmetro não encontrada para o comando: %s\n", command_name);
            }
            return;
        }
    }
    printf("Comando não encontrado: %s\n", command_name);
}

bool isCommand(const char* n) {
    for (int i = 0; i < sizeof(commandNames) / sizeof(commandNames[0]); i++) {
        if (strcmp(commandNames[i], n) == 0) {
            return true;
        }
    }
    return false;
}


void execute_commands(char* commands) {
    Serial.println(commands);

    int num_tokens;
    char** p = split(commands, "\n", &num_tokens);
    for (int i = 0; i < num_tokens - 2; i++) {
        char* line = strip(p[i]);
        if (line == NULL || line[0] == '\0' || line[0] == '#') {
            continue;
        }
        //Serial.println(line);
        if (strstr(line, "=") != NULL) {
            handle_assignment(line);
        } else {
            int partNumber;
            char** part = split(line, " ", &partNumber);
            char* command = part[0];
            char** args = new char*[partNumber];

            for (int kk = 1; kk < partNumber; kk++) {

                args[kk - 1] = part[kk];
            }

           

            if (strcmp(command, "WHILE") == 0 || strcmp(command, "IF") == 0 || strcmp(command, "FUNC") == 0 || strcmp(command, "FUNC_BTNSTATE") == 0) {
                i = callFunctionByNameINT(command, (const char**)args, i, (const char**)p,partNumber,num_tokens);
            } else {
                if (isCommand(command)) {
                    callFunctionByName(command, (const char**)args,partNumber);
                }
            }
            free(part);
            free(args);
        }
    }

    free(p);
}

void execute_script(const char* script_path) {
    char* code =
        "INT x 0\n"
        "FLOAT y 0.0\n"
        "STR name 'John'\n"
        "MATRIX mat 3 3\n"
        "VECTOR vec 3\n"
        "# Declare the variable\n"
        "INT counter 0\n"
        "\n"
        "# Loop while counter is less than 5\n"
        "WHILE counter < 5\n"
        "    PRINT 'counter:' counter\n"
        "    counter = counter + 1\n"
        "    PRINT 'Next counter:' counter\n" 
        "ENDWHILE\n"
        "PRINT 'END'\n";
    execute_commands(code);
}