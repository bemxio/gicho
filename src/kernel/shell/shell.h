#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VARIABLE_COUNT 32 // maximum number of variables that can be defined
#define LINE_COUNT 64 // maximum number of lines that can be defined in a script
#define INPUT_BUFFER_SIZE 256 // length of buffer for "input" command

typedef enum {
    SHELL_TYPE_INTEGER, // 16-bit unsigned integer
    SHELL_TYPE_STRING, // null-terminated string
    SHELL_TYPE_BOOLEAN // true or false
} shell_type_t; // variable type

typedef struct {
    char* name; // null if variable is not defined
    shell_type_t type;
    void* value; // dereferenced depending on type
} shell_var_t; // variable

typedef struct {
    uint16_t index; // line number, zero if line is not defined
    uint8_t length; // length of buffer
    char* buffer; // command to execute
} shell_line_t; // line of script

// TODO: replace booleans with a single flag variable
typedef struct shell_t {
    char* token; // current token parsed by command functions
    shell_var_t variables[VARIABLE_COUNT]; // list of shell variables
    shell_line_t script[LINE_COUNT]; // list of commands in script
    void* output; // pointer to output variable for commands, used by "eval", "peek" and "input"
    bool in_script; // true if currently running a script
    size_t line_index; // index of current line when running a script
    bool print_without_newline; // true if "print" command should not print a newline at the end, used by "input"
} shell_t; // shell state

// variable functions
shell_var_t* shell_var_get(shell_t* shell, char* name); // returns variable with given name, or NULL if not found
shell_var_t* shell_var_set(shell_t* shell, char* name, shell_type_t type, void* value, bool allocate_value); // creates or updates variable with given name, type and value, returns NULL if variable limit is reached
void shell_var_unset(shell_t* shell, char* name); // unsets variable with given name, if name is NULL unsets all variables

// script functions
shell_line_t* shell_line_get(shell_t* shell, uint16_t index); // returns line with given index, or NULL if not found
shell_line_t* shell_line_set(shell_t* shell, uint16_t index, char* buffer); // creates or updates line with given index and buffer, returns NULL if line limit is reached
void shell_line_unset(shell_t* shell, uint16_t index); // unsets line with given index, if index is zero unsets all lines
// TODO: implement sort() instead of sorting in shell_line_sort()
void shell_line_sort(shell_t* shell); // sorts lines in ascending order by index, used by "run" and "list" commands
void shell_line_fix(shell_t* shell, uint16_t index); // replaces null characters in line buffer with spaces, used by "run" command to fix lines that were modified by "input" command

#endif