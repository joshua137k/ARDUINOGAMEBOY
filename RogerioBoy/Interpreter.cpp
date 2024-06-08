#include <Arduino.h>
#include "Interpreter.h"
#include "video_display.h"


Dictionary variable(100);

#include <stdint.h>

const uint8_t colors[] = {
    0x00, // Preto (se precisar do valor para preto)
    0x03, // Azul
    0x1C, // Verde
    0x1F, // Ciano
    0xE0, // Vermelho
    0xE3, // Magenta
    0xFC, // Amarelo
    0xFF  // Branco
};

void updateFrame() {
    videoOut.waitForFrame();
}

float getValueFromDict(const char* key) {
    ValueType type;
    Value value = variable.getValue(key, type);

    switch (type) {
        case INT:
            return (float)value.intValue;
        case FLOAT:
            return value.floatValue;
        default:
            return 0.0; // Aqui podemos lançar um erro ou lidar de forma diferente
    }
}


char* strip(char* str) {
    while (isspace(*str)) str++;
    if (*str == 0) return str;
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int maxToken(const char* str, char* delimit) {
    int count = 0;
    while (*str) {
        if (*str == *delimit) {
            count++;
        }
        str++;
    }
    return count + 1;
}

char** split(const char* str, char* delimit, int* num_token) {
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
    char* expr_copy = strdup(expression);
    char* token = strtok(expr_copy, " ");
    float result = 0.0;
    char last_op = '+';
    while (token != NULL) {
        if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            last_op = token[0];
        } else {
            float value;
            ValueType type;
            if (variable.exists(token)) {
                value =getValueFromDict(token);
            } else {
                value = atof(token);
            }
            switch (last_op) {
                case '+': result += value; break;
                case '-': result -= value; break;
                case '*': result *= value; break;
                case '/': result /= value; break;
            }
        }
        token = strtok(NULL, " ");
    }
    free(expr_copy);
    return result;
}

bool compareValues(float leftValue, const char* op, float rightValue) {
    if (strcmp(op, ">") == 0) return leftValue > rightValue;
    if (strcmp(op, "<") == 0) return leftValue < rightValue;
    if (strcmp(op, ">=") == 0) return leftValue >= rightValue;
    if (strcmp(op, "<=") == 0) return leftValue <= rightValue;
    if (strcmp(op, "==") == 0) return leftValue == rightValue;
    if (strcmp(op, "!=") == 0) return leftValue != rightValue;
    return false;
}


bool evaluate_condition(const char** condition, int partNumber) {
    float leftValue = 0;
    float rightValue = 0;
    const char* op = nullptr;
    bool logicalAnd = true;  // default to AND operation if none specified

    for (int i = 0; i < partNumber-1; i++) {
        if (strcmp(condition[i], "and") == 0) {
            logicalAnd = true;
        } else if (strcmp(condition[i], "or") == 0) {
            logicalAnd = false;
        } else if (i % 4 == 0) {  // this should be the left value or a variable
            if (variable.exists(condition[i])) {
                leftValue = getValueFromDict(condition[i]);
            } else {
                leftValue = atof(condition[i]);
            }
        } else if (i % 4 == 1) {  // this should be the operator
            op = condition[i];
        } else if (i % 4 == 2) {  // this should be the right value or a variable
            if (variable.exists(condition[i])) {
                rightValue = getValueFromDict( condition[i]);
            } else {
                rightValue = atof(condition[i]);
            }

            // Evaluate the expression
            bool result = compareValues(leftValue, op, rightValue);
            if (logicalAnd) {
                if (!result) return false;  // AND operation, if any false, return false
            } else {
                if (result) return true;  // OR operation, if any true, return true
            }
        }
    }
    return logicalAnd;  // if all AND conditions passed, return true
}

int btn_state(const char** args, int i, const char** lines, int partNumber, int num_tokens) {
    Serial.print("Button state with args: ");
    Serial.println(args[0]);  // Print the first argument
    return i + 1;
}

int while_loop(const char** args, int i, const char** lines, int partNumber, int num_tokens) {
    String block_commands = "";
    int nested_w = 0;
    i++;

    // Encontrar o bloco de comandos dentro do while
    while (i < num_tokens) {
        char* current_line = strip((char*)lines[i]);
        if (strncmp(current_line, "WHILE", 5) == 0) {  // Corrige a comparação para "WHILE"
            nested_w++;
        } else if (strncmp(current_line, "ENDWHILE", 8) == 0) {  // Corrige a comparação para "ENDWHILE"
            if (nested_w == 0) {
                break;
            }
            nested_w--;
        }
        block_commands += String(strip((char*)lines[i])) + "\n";
        i++;
    }

    // Executar o bloco de comandos repetidamente enquanto a condição for verdadeira
    while (evaluate_condition(args, partNumber)) {
        execute_commands(block_commands.c_str());
    }

    // Retornar o índice do fim do bloco while
    return i;
}


void handle_assignment(const char* line) {
    char* line_copy = strdup(line);
    char* var_name = strtok(line_copy, "=");
    char* expression = strtok(NULL, "=");
    
    if (var_name && expression) {
        var_name = strip(var_name);
        expression = strip(expression);
        float value = evaluateMath(expression);
        ValueType type;
        Value v = variable.getValue(var_name, type);
        
        switch (type) {
            case INT:
                v.intValue = (int)value;
                variable.change(var_name, v, type);
                break;
            case FLOAT:
                v.floatValue = value;
                variable.change(var_name, v, type);
                break;
        }
    }
    free(line_copy);
}

void set_color(const char** args, int partNumber) {
    Serial.print("Set color with args: ");
    Serial.println(args[0]);
    // 3 args x,y,color
    uint8_t color = colors[0];
    if (strcmp(args[2],"red")==0){ color = colors[4];}
    int x = (int)getValueFromDict(args[0]); 
    int y=(int)getValueFromDict(args[1]);
    setPixel(x, y, color); 
    

    

    
}

int if_statement(const char** args, int i, const char** lines, int partNumber, int num_tokens) {
    String block_commands = "";
    int nested_ifs = 0;
    i++;

    while (i < num_tokens) {
        char* current_line = strip((char*)lines[i]);
        if (strncmp(current_line, "IF", 2) == 0) {
            nested_ifs++;
        } else if (strncmp(current_line, "ENDIF", 5) == 0) {
            if (nested_ifs == 0) {
                break;
            }
            nested_ifs--;
        }
        block_commands += String(strip((char*)lines[i])) + "\n";
        i++;
    }

    if (evaluate_condition(args, partNumber)) {
        execute_commands(block_commands.c_str());
    }

    return i;
}

void print_message(const char** args, int partNumber) {
    String message = "";
    bool is_str = false;
    //partNumber--;
    
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

    Serial.println("SERIAL.PRINT: "+message);
}

void get_matrix_value(const char** args, int partNumber) {
    Serial.print("Get matrix value with args: ");
    Serial.println(args[0]);
}

void set_matrix_value(const char** args, int partNumber) {
    Serial.print("Set matrix value with args: ");
    Serial.println(args[0]);
}

int define_function(const char** args, int i, const char** lines, int partNumber, int num_tokens) {
    Serial.print("Define function with args: ");
    Serial.println(args[0]);
    return i + 1;
}

void call_function(const char** args, int partNumber) {
    Serial.print("Call function with args: ");
    Serial.println(args[0]);
}

void call_BTNfunction(const char** args, int partNumber) {
    Serial.print("Call button function with args: ");
    Serial.println(args[0]);
}

void declare_int(const char** args, int partNumber) {
    variable.addItem(args[0], atoi(args[1]));
}

void declare_float(const char** args, int partNumber) {
    variable.addItem(args[0], (float)atof(args[1]));
}

void declare_str(const char** args, int partNumber) {
    variable.addItem(args[0], args[1]);
}

void declare_matrix(const char** args, int partNumber) {
    int rows = atoi(args[1]);
    int cols = atoi(args[2]);
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    variable.addItem(args[0], matrix);
}

void declare_vector(const char** args, int partNumber) {
    int* vec = new int[atoi(args[1])];
    variable.addItem(args[0], vec);
}

void evaluate(const char** expression, int partNumber) {
    Serial.print("Evaluate expression: ");
    Serial.println(expression[0]);
}

const char* commandNames[] = {
    "SET",
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
    {"SET", NULL, set_color},//OK
    {"IF", if_statement, NULL},//OK
    {"PRINT", NULL, print_message},//OK
    {"GETMATRIXVALUE", NULL, get_matrix_value},
    {"SETMATRIXVALUE", NULL, set_matrix_value},
    {"FUNC", define_function, NULL},
    {"CALL", NULL, call_function},
    {"INT", NULL, declare_int},//OK
    {"FLOAT", NULL, declare_float},//OK
    {"STR", NULL, declare_str},//OK
    {"MATRIX", NULL, declare_matrix},//OK
    {"VECTOR", NULL, declare_vector}//OK
};

int callFunctionByNameINT(const char* command_name, const char** args, int i, const char** lines, int partNumber, int num_tokens) {
    for (int j = 0; j < sizeof(commands) / sizeof(Command); j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            if (commands[j].runFuncInt != NULL) {
                return commands[j].runFuncInt(args, i, lines, partNumber, num_tokens);
            } else {
                Serial.printf("Função com três parâmetros não encontrada para o comando: %s\n", command_name);
            }
        }
    }
    Serial.printf("Comando não encontrado: %s\n", command_name);
    return -1;
}

void callFunctionByName(const char* command_name, const char** args, int partNumber) {
    for (int j = 0; j < sizeof(commands) / sizeof(Command); j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            if (commands[j].runFuncVoid != NULL) {
                commands[j].runFuncVoid(args, partNumber);
            } else {
                Serial.printf("Função com um parâmetro não encontrada para o comando: %s\n", command_name);
            }
            return;
        }
    }
    Serial.printf("Comando não encontrado: %s\n", command_name);
}

bool isCommand(const char* n) {
    for (int i = 0; i < sizeof(commandNames) / sizeof(commandNames[0]); i++) {
        if (strcmp(commandNames[i], n) == 0) {
            return true;
        }
    }
    return false;
}

void execute_commands(const char* commands) {
    
    
    int num_tokens;
    char** p = split(commands, "\n", &num_tokens);
    for (int i = 0; i < num_tokens -1; i++) {
       
        
        char* line = strip(p[i]);
        if (line == NULL || line[0] == '\0' || line[0] == '#') {
            continue;
        }

        if (strstr(line, "=") != NULL) {
            handle_assignment(line);
        } else {
            int partNumber;
            char** part = split(line, " ", &partNumber);
            char* command = part[0];


            if (strcmp(command,"UPDATEFRAME")==0){
                updateFrame();
            }
            else{


                char** args = new char*[partNumber - 1];

                for (int kk = 1; kk < partNumber; kk++) {
                    args[kk - 1] = part[kk];
                }

                if (strcmp(command, "WHILE") == 0 || strcmp(command, "IF") == 0 || strcmp(command, "FUNC") == 0 || strcmp(command, "FUNC_BTNSTATE") == 0) {
                    i = callFunctionByNameINT(command, (const char**)args, i, (const char**)p, partNumber, num_tokens);
                } else {
                    if (isCommand(command)) {
                        callFunctionByName(command, (const char**)args, partNumber - 1);
                    }
                }
                
                delete[] args;
            }
            free(part);
        
            
        }
    }
    free(p);
}

void execute_script(const char* script_path) {


    updateFrame();
    const char* code =
        "INT x 50\n"
        "INT y 50\n"
        "# Declare the variable\n"
        "# Loop while counter is less than 5\n"
        "WHILE x < 100\n"
        "    PRINT 'x:' x\n"
        "    x = x + 1\n"
        "    y = y + 1\n"
        "    SET x y red\n"
        "ENDWHILE\n"
        "UPDATEFRAME\n"
        "PRINT 'END'";

    Serial.println("----------------------\n");
    Serial.println(code);
    Serial.println("----------------------\n");
    execute_commands(code);
}



