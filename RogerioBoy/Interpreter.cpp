#include <Arduino.h>
#include "Interpreter.h"
#include "video_display.h"

const size_t capacity = JSON_OBJECT_SIZE(50) + 500;
DynamicJsonDocument variable(capacity);
DynamicJsonDocument functions(capacity);
DynamicJsonDocument functionsbtn(capacity);

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

// Function to strip whitespace from a string
char* strip(char* str) {
    while (isspace(*str)) str++;
    if (*str == 0) return str;
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

// Function to count tokens in a string based on delimiter
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

// Function to split a string into tokens
char** split(const char* str, char* delimit, int* num_token) {
    char* str_copy = strdup(str);
    int token_count = maxToken(str, delimit);
    
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
    *num_token = index;
    free(str_copy);
    return parts;
}

// Function to evaluate a mathematical expression
float evaluateMath(const char* expression) {
    char* expr_copy = strdup(expression);
    char* token = strtok(expr_copy, " ");
    
    float result = 0.0;
    char last_op = '+';
    float value = 0;
   
    while (token != NULL) {
        if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            last_op = token[0];
        } else {
            if (variable.containsKey(token)) {
                value = variable[token].as<float>();
            }
            else {value = atof(token);}
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

// Function to compare two values based on an operator
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


    if (variable.containsKey(condition[0])) {

        leftValue = variable[condition[0]].as<float>();
    } else {
        leftValue = atof(condition[0]);
    }

    op = condition[1];

    if (variable.containsKey(condition[2])) {
        rightValue = variable[condition[2]].as<float>();
    } else {
        rightValue = atof(condition[2]);
    }


    bool result = compareValues(leftValue, op, rightValue);

    return result;  
}

void cleanScreen(const char** args, int partNumber) {
    clean();
}

void timedelay(const char** args, int partNumber) {
    int t = atoi(args[0]);
    delay(t);

}

int btn_state(const char** args, int i, const char** lines, int partNumber, int num_tokens) {
    const char* func_name = args[0];
    partNumber--;

    
    String block_commands = "";
    i++;
    while (i < num_tokens && strcmp(strip((char*)lines[i]), "ENDBTNFUNC") != 0) {
        block_commands += String(strip((char*)lines[i])) + "\n";
        i++;
    }

    JsonArray param_array = functionsbtn[func_name].createNestedArray("parameters");
    for (int j = 1; j < partNumber; j++) {
        param_array.add(strdup(args[j]));
    }
    functionsbtn[func_name]["block_commands"] = block_commands;



    return i;
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
        variable[var_name] = value;
    }
    free(line_copy);
}

void set_color(const char** args, int partNumber) {

    // 4 args x,y,size,color
    uint8_t color = colors[0];
    if (strcmp(args[3],"red")==0){ color = colors[4];}
    int x =0;
    int y =0;
    int size = 0;
    if (variable.containsKey(args[0])){
        x = variable[args[0]].as<int>();
    }else{x = atoi(args[0]);}
    if (variable.containsKey(args[1])){
        y = variable[args[1]].as<int>();
    }else{y = atoi(args[1]);}
    if (variable.containsKey(args[2])){
        size = variable[args[2]].as<int>();
    }else{size = atoi(args[2]);}
    


    videoOut.waitForFrame();
    drawSquare(x, y, size, color);
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
    for (int i = 0; i < partNumber; i++) {
        if (args[i] == NULL || strcmp(args[i], "\n") == 0) {
            break;
        }
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
            message += String(variable[args[i]].as<float>());
        }
    }

    Serial.println("SERIAL.PRINT: " + message);
}

void filterCommonElements(DynamicJsonDocument& v, DynamicJsonDocument& c) {
    const char* keysToRemove[20];
    int keysCount = 0;

    for (JsonPair kv : v.as<JsonObject>()) {
        const char* key = kv.key().c_str();

        if (!c.containsKey(key)) {
            if (keysCount < 20) {
                keysToRemove[keysCount++] = key;
            }
        }
    }

    for (int i = 0; i < keysCount; ++i) {
        v.remove(keysToRemove[i]);
    }
}

int define_function(const char** args, int i, const char** lines, int partNumber, int num_tokens) {
    const char* func_name = args[0];
    partNumber--;

    
    String block_commands = "";
    i++;
    while (i < num_tokens && strcmp(strip((char*)lines[i]), "ENDFUNC") != 0) {
        block_commands += String(strip((char*)lines[i])) + "\n";
        i++;
    }

    JsonArray param_array = functions[func_name].createNestedArray("parameters");
    for (int j = 1; j < partNumber; j++) {
        param_array.add(strdup(args[j]));
    }
    functions[func_name]["block_commands"] = block_commands;



    return i;
}

void call_BTNfunction(const char** args, int partNumber) {
    const char* func_name = args[0];

    if (functionsbtn.containsKey(func_name)) {
        DynamicJsonDocument cloneDoc(capacity);
        cloneDoc.set(variable);


        JsonArray params = functionsbtn[func_name]["parameters"].as<JsonArray>();
        for (int i = 0; i < params.size(); i++) {
            if (variable.containsKey( args[i + 1])){
                variable[params[i].as<const char*>()] = variable[args[i + 1]];
            }else{
                variable[params[i].as<const char*>()] = atof(args[i + 1]);
            }
            
            
        }
        execute_commands(functionsbtn[func_name]["block_commands"].as<String>().c_str());

        filterCommonElements(variable,cloneDoc);


        

        
    } else {
        Serial.printf("Função não encontrada: %s\n", func_name);
    }
}

void declare_int(const char** args, int partNumber) {
    variable[args[0]] = atoi(args[1]);
}

void declare_float(const char** args, int partNumber) {
    variable[args[0]] = (float)atof(args[1]);
}

void declare_str(const char** args, int partNumber) {
    variable[args[0]] = args[1];
}

void declare_vector(const char** args, int partNumber) {
    JsonArray array = variable[args[0]].createNestedArray("vetor");
}

void call_function(const char** args, int partNumber) {
    const char* func_name = args[0];

    if (functions.containsKey(func_name)) {
        DynamicJsonDocument cloneDoc(capacity);
        cloneDoc.set(variable);


        JsonArray params = functions[func_name]["parameters"].as<JsonArray>();
        for (int i = 0; i < params.size(); i++) {
            if (variable.containsKey( args[i + 1])){
                variable[params[i].as<const char*>()] = variable[args[i + 1]];
            }else{
                variable[params[i].as<const char*>()] = atof(args[i + 1]);
            }
            
            
        }
        execute_commands(functions[func_name]["block_commands"].as<String>().c_str());



        filterCommonElements(variable,cloneDoc);

        

        
    } else {
        Serial.printf("Função não encontrada: %s\n", func_name);
    }
}

const char* commandNames[] = {
    "SET", // COOLOCAR QUADRADO NA TELA (X,Y,SIZE,COLOR)
    "PRINT", // DAR PRINT NO TERMINAL
    "CALL", // CHAMAR FUNÇÂO
    "INT", // DECLARAR UM INT
    "FLOAT",// DECLARAR UM FLOAT
    "STR",// DECLARAR UM STR
    "VECTOR",// DECLARAR UM VECTOR
    "CLEAN", //LIMPAR TELA
    "DELAY"// FAZ UMA PAUSA
};

Command commands[] = {
    {"FUNC_BTNSTATE", btn_state, NULL},//OK
    {"WHILE", while_loop, NULL},//ok
    {"SET", NULL, set_color},//OK
    {"IF", if_statement, NULL},//OK
    {"PRINT", NULL, print_message},//OK
    {"FUNC", define_function, NULL},//OK
    {"CALL", NULL, call_function},//OK
    {"INT", NULL, declare_int},//OK
    {"FLOAT", NULL, declare_float},//OK
    {"STR", NULL, declare_str},//OK
    {"VECTOR", NULL, declare_vector},//OK
    {"CLEAN", NULL, cleanScreen},//OK
    {"DELAY", NULL, timedelay} //OK
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
    for (int i = 0; i < num_tokens ; i++) {
        char* line = strip(p[i]);
        if (line == NULL ){
            break;
        }
        if ( line[0] == '\0' || line[0] == '#') {continue;}
        if (strstr(line, "=") != NULL) {
            handle_assignment(line);
        } else {
            int partNumber;
            char** part = split(line, " ", &partNumber);
            char* command = part[0];
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
            free(part);
        }
    }
    free(p);
}

void execute_script(const char* script_path) {
    Serial.println("----------------------------------------------");
    Serial.println(script_path);
    Serial.println("----------------------------------------------");

    execute_commands(script_path);
}
