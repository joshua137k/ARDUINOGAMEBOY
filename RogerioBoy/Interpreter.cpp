#include <Arduino.h>
#include "Dictionary.h"
#include "Interpreter.h"


Dictionary variable;




int btn_state(const char** args, int i, const char** lines) {
    Serial.print("Button state with args: ");
    Serial.println(args[0]);  // Print the first argument
    return i++;
}

int while_loop(const char** args, int i, const char** lines) {
    Serial.print("While loop with args: ");
    return i++;
}

void handle_assignment(const char* line) {
    Serial.print("Handle assignment with line: ");
}

void set_color(const char** args) {
    Serial.print("Set color with args: ");
}

int if_statement(const char** args, int i, const char** lines) {
    Serial.print("If statement with args: ");
    return i++;
}

void input_var(const char** args) {
    Serial.print("Input variable with args: ");
}

void print_message(const char** args) {
    Serial.print("Print message with args: ");
}

void get_matrix_value(const char** args) {
    Serial.print("Get matrix value with args: ");
}

void set_matrix_value(const char** args) {
    Serial.print("Set matrix value with args: ");
}

int define_function(const char** args, int i, const char** lines) {
    Serial.print("Define function with args: ");
    return i++;
}

void call_function(const char** args) {
    Serial.print("Call function with args: ");
}

void call_BTNfunction(const char** args) {
    Serial.print("Call button function with args: ");
}

void declare_int(const char** args) {
    Serial.print("args[1]:");Serial.println(args[1]);
    variable.addItem(args[0], atoi(args[1]));
    int i = variable.getValue(args[0]).intValue;
    Serial.print("->:");Serial.println(i);
    Serial.print("Declare int with args: ");
}

void declare_float(const char** args) {
    Serial.print(args[0]);Serial.print(",");Serial.println(args[1]);
    Serial.print("Declare float with args: ");
}

void declare_str(const char** args) {
    Serial.print(args[0]);Serial.print(",");Serial.println(args[1]);
    Serial.print("Declare string with args: ");
}

void declare_matrix(const char** args) {
    Serial.print(args[0]);Serial.print(",");Serial.print(args[1]),Serial.print(",");Serial.println(args[2]);
    Serial.print("Declare matrix with args: ");
}

void declare_vector(const char** args) {
    Serial.print(args[0]);Serial.print(",");Serial.println(args[1]);
    Serial.print("Declare vector with args: ");
}



void evaluate(const char** expression) {

    Serial.print("Evaluate expression: ");
}

void evaluate_condition(const char** condition) {
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





int callFunctionByNameINT(const char* command_name, const char** args, int i, const char** lines) {
    for (int j = 0; j < sizeof(commands) / sizeof(Command); j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            if (commands[j].runFuncInt != NULL) {
                 
                return commands[j].runFuncInt(args, i, lines);
            } else {
                printf("Função com três parâmetros não encontrada para o comando: %s\n", command_name);
            }
        }
    }
    printf("Comando não encontrado: %s\n", command_name);
    return -1;
}

void callFunctionByName(const char* command_name, const char** args) {
    for (int j = 0; j < sizeof(commands) / sizeof(Command); j++) {
        if (strcmp(commands[j].name, command_name) == 0) {
            if (commands[j].runFuncVoid != NULL) {
                
                commands[j].runFuncVoid(args);
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

void execute_commands(char* commands) {
    Serial.println(commands);

    int num_tokens;
    char** p = split(commands, "\n", &num_tokens);
    for (int i = 0; i < num_tokens - 2; i++) {
        char* line = strip(p[i]);
        if (line == NULL || line[0] == '\0' || line[0] == '#') {
            continue;
        }
        Serial.println(line);
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
                i = callFunctionByNameINT(command, (const char**)args, i, (const char**)p);
            } else {
                if (isCommand(command)) {
                    callFunctionByName(command, (const char**)args);
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
        "    PRINT counter\n"
        "    counter = counter + 1\n"
        "ENDWHILE\n"
        "PRINT \"END\"\n";
    execute_commands(code);
}