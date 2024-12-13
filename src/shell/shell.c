#include "shell.h"
#include "vga.h"
#include "string_utils.h"
#include "keyboard.h"

#define MAX_INPUT_SIZE 256
#define MAX_HISTORY_SIZE 10 

const char* all_commands[] = {
    "echo",
    "amogus", 
    "flappy",
    "clear",
    "help",
    "bigroos",
    NULL
};

typedef struct { 
    char command[MAX_INPUT_SIZE];
} CommandHistory;

static CommandHistory command_history[MAX_HISTORY_SIZE];
static int history_count = 0;
static int history_position = -1;

static char input_buffer[MAX_INPUT_SIZE];
static int input_position = 0;


#define amogus "\n"\
"               AAAAAAAAAAAA            \n"\
"             AA            AA          \n"\
"            AA               A         \n"\
"           AA       AAAAAAAAAAAA       \n"\
"          AA     AA            AAA     \n"\
"       AAAAA     A              AA     \n"\
"    AAA   A      A               A     \n"\
"    AA    A       A             AA     \n"\
"   AA     A        AAAAAAAAAAAAAA      \n"\
"   AA     A                    AA      \n"\
"   AA     A                    AA      \n"\
"   AA     A                    A       \n"\
"   AA     A                    A       \n"\
"   AA     A                   AA       \n"\
"   AA     A                   AA       \n"\
"    AA    A                   AA       \n"\
"     AAAAAA         AAA       A        \n"\
"          A        AA AA     AA        \n"\
"          AA       AA AA     AA        \n"\
"           AAAAAAAAA   AAAAAAA         \n"

#define flappy "\n"  \
"              FFFFFFFFFFFFF           \n" \
"         FFFFF      FF     FF         \n" \
"       FF         FF         FF       \n" \
"   FFFFFFF        FF         FFFF     \n" \
"   FFFFFFFF       FF       FF  FF     \n" \
"FFF        FFF    FF       FF  FF     \n" \
"FFF           FF    FF         FF     \n" \
"FFF           FF      FFFFFFFFFFFFFF  \n" \
"   FF      FFF      FFF            FFF\n" \
"    FFFFFFFF      FFFFFFFFFFFFFFFFFF  \n" \
"     FFFFFF       FFFFFFFFFFFFFFFFFF  \n" \
"     FF             FFFFFFFFFFFFFFFF  \n" \
"       FFFFF          FFFFFFFFFFF     \n" \
"           FFFFFFFFFFF                \n"

#define BigroOS "\n" \
" ____  _                  ___  ____  \n"\
"| __ )(_) __ _ _ __ ___  / _ \\/ ___| \n"\
"|  _ \\| |/ _` | '__/ _ \\| | | \\___ \\ \n"\
"| |_) | | (_| | | | (_) | |_| |___) |\n"\
"|____/|_|\\__, |_|  \\___/ \\___/|____/ \n"\
"         |___/                       \n"\
"\n" \
"By Bisher Almasri\n" \
"Version 0.0.1\n" \
"\n"

void shell_initialize(void) {
    terminal_initialize();
    terminal_writestring(BigroOS);
    terminal_writestring("BigroOS Shell Initialized\n");
    terminal_writestring("> ");
}

void user_input(char c) {
    static int escape_sequence = 0;
    if (escape_sequence == 0 && c == '\033') { 
        escape_sequence = 1;
        return;
    }
    if (escape_sequence == 1 && c == '[') {
        escape_sequence = 2;
        return;
    }

        if (c == KEY_UP) {
        if (history_position < history_count - 1) {
            while (input_position > 0) {
                terminal_backspace();
                input_position--;
            }
            history_position++;
            strcpy(input_buffer, command_history[history_position].command);
            input_position = strlen(input_buffer);
            terminal_writestring(input_buffer);
        }
        return;
    } 

    if (c == '\t') {
        input_buffer[input_position] = '\0';
        for (int i = 0; all_commands[i] != NULL; i++) {
            if (starts_with(all_commands[i], input_buffer)) {
                while (input_position > 0) {
                    terminal_backspace();
                    input_position--;
                }
                strcpy(input_buffer, all_commands[i]);
                input_position = strlen(input_buffer);
                terminal_writestring(input_buffer);
                break;
            }
        }
        return;
    }

    else if (c == KEY_DOWN) {  
        if (history_position >= 0) {
            while (input_position > 0) {
                terminal_backspace();
                input_position--;
            }
            history_position--;
            if (history_position >= 0) {
                strcpy(input_buffer, command_history[history_position].command);
                input_position = strlen(input_buffer);
                terminal_writestring(input_buffer);
            }
        }
        return;
    }


    if (c == '\n') {
        input_buffer[input_position] = '\0';
        
        if (input_position > 0) {
            if (history_count >= MAX_HISTORY_SIZE) {
                for (int i = MAX_HISTORY_SIZE - 1; i > 0; i--) {
                    strcpy(command_history[i].command, command_history[i - 1].command);
                }
                history_count = MAX_HISTORY_SIZE;
            } else {
                for (int i = history_count; i > 0; i--) {
                    strcpy(command_history[i].command, command_history[i - 1].command);
                }
                history_count++;
            }
            strcpy(command_history[0].command, input_buffer);
        }
        
        execute_command(input_buffer);
        input_position = 0;
        history_position = -1;
        terminal_writestring("> ");
    } else if (c == '\b') {
        if (input_position > 0) {
            terminal_backspace();
            input_position--;
        }
    } else if (input_position < MAX_INPUT_SIZE - 1) {
        input_buffer[input_position++] = c;
        terminal_putchar(c);
    }
}

void execute_command(const char* command) {
    if (compare_strings(command, "clear") == 0) {
        terminal_initialize(); 
    } else if (starts_with(command, "echo")) {
        terminal_writestring("\n");
        terminal_writestring(command + 6);
        terminal_writestring("\n");
    } else if (compare_strings(command, "amogus") == 0) {
        terminal_writestring(amogus);
    } else if (compare_strings(command, "flappy") == 0) {
        terminal_writestring(flappy);
    } else if (compare_strings(command, "") == 0){
        terminal_writestring("\n");
    } else if (compare_strings(command, "bigroos") == 0) {
        terminal_writestring(BigroOS);
    } else if (compare_strings(command, "help") == 0) {
        terminal_writestring("\n");
        terminal_writestring("echo - prints a string\n");
        terminal_writestring("amogus - prints the amogus\n");
        terminal_writestring("flappy - prints the flappy bird\n");
        terminal_writestring("clear - clears the screen\n");
        terminal_writestring("help - prints this help\n");
        terminal_writestring("bigroos - prints the BigroOs startup message\n");
    }  else {
        terminal_writestring("\n");
        terminal_writestring("Unknown command: ");
        terminal_writestring(command);
        terminal_writestring("\n");
    }
}

int compare_strings(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return 0;
        }
    }
    return 1;
}
