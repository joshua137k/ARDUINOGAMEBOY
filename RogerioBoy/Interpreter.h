#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef int (*CommandFuncInt)(const char**, int, const char**,int ,int );
typedef void (*CommandFuncVoid)(const char**,int );

typedef struct {
    const char* name;
    CommandFuncInt runFuncInt;
    CommandFuncVoid runFuncVoid;
} Command;

void execute_script(const char* script_path);

void execute_commands(const char* commands);

void call_BTNfunction(const char** args);


#endif