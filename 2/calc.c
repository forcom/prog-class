///
///  calc.c
///      A Common Calculator with a Memory Feature.
///
///  Compile :
///      Compile with a standard UNIX C Compiler with curses library.
///      $ cc calc.c -o calc -lcurses
///
///  Usage :
///      To input a number, you can use 0~9 and . keys.
///      To calculate, you can use +, -, *, /, = keys.
///      To clear current number, you can use D or d keys.
///      To clear all result, you can use C or c keys.
///      To memorize current number, you can use P or p keys to add to memory, Q or q keys to subtract from memory.
///      To use memorized number, you can use M or m keys.
///      To clear memory, you can use R or r keys.
///
///  Written by Yoon, SeungYong, 2012. 06. 15.
///
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <memory.h>

// Define Calculation Tasks
#define NOTHING 0
#define PLUS 1
#define MINUS 2
#define TIME 3
#define DIVIDE 4
#define EQUAL 5

void num_clear();
void num_init();

long double memory = 0.0; // Memory
long double result = 0.0; // Result
short cur_task = 0; // Is calculating?
short cur_print = 0; // Printed with input number(0) or with result(1)
short dot = 0; // Is dot added?
short mem = 0; // Is memory using?
short ext = 0; // Is there any exit sign?
short lock = 0; // Is locked because of overflow or divide 0?

short inp_sig = 0; // If memory is used and the value is under 0, 1 otherwise 0.
char inpnum [ 12 ]; // Input number
int length; // Length of input number.

/// void print_init ()
///     description : Print a initial background.
void print_init() {
    initscr(); // Start using curses library.
    printw("+====================+\n");
    printw("|   +------------+   |\n");
    printw("|   |            |   |\n");
    printw("|   +------------+   |\n");
    printw("+--------------------+\n");
    printw("| M R  M C  M +  M - |\tMR: M\tMC: R\tM+: P\tM-: Q\n");
    printw("|                    |\n");
    printw("| CLR  C E        X  |\tCLR: C\tCE: D\tExit: X\n");
    printw("|                    |\n");
    printw("|  1    2    3    +  |\n");
    printw("|                    |\n");
    printw("|  4    5    6    -  |\n");
    printw("|                    |\n");
    printw("|  7    8    9    *  |\n");
    printw("|                    |\n");
    printw("|  0    .    =    /  |\n");
    printw("+====================+\n");
}

/// void print_term()
///     description : Print a terminating message.
void print_term() {
    printw("Thank you for using Calculator!! \n");
    endwin(); // Finish using curses library.
}

/// char * strrev(char * str)
///     description : Reverse a string.
///     argument :
///         str : A string to reverse
///     result : Reversed string. Actually it is same with input string.
char * strrev(char * str) {
    char * f = str - 1, * t = str; // f: head of string, t: tail of string

    while (* t) ++ t; // find the end of string.

    while (++f < --t) // Swap head and tail.
        * f ^= *t ^= *f ^= *t;

    return str;
}

/// char * number_to_string(long double number)
///     description : Convert an absolute value of a real number to char array in 10 digits.
///     argument :
///         number : A number to convert.
///     result : Converted character array.
char * number_to_string(long double number) {
    if (number < 0) number = -number; // get an absolute value.
    number += 0.000000005; // upper under 10 digits.

    char res [ 15 ]; // result string
    char r1 [ 11 ]; // decimal part
    char r2 [ 11 ]; // fraction part
    long long r = number; // get decimal part
    long double z = number - r; // get fraction part
    char * ptr, *ptr2;

    res [0] = 0;

    /// Change integer to char array r1.
    ptr = r1;

    // Get the last digit and paste to char array.
    while (r != 0 && ptr - r1 < 10) {
        * ptr++ = r % 10 + '0';
        r /= 10;
    }

    // If decimal part is over 10 digits, show error.
    if (r != 0) {
        lock = 1;
        return "!OVERFLOW!";
    }

    // If decimal part is zero, add 0.
    if (ptr == r1) {
        * ptr++ = '0';
    }
    * ptr = 0;

    strrev(r1);

    /// Change double to char array r2
    ptr = r2;

    // Get the first 8 digits and paste to char array.
    while ((ptr - r2) < 8) {
        z *= 10;
        * ptr++ = (int) z + '0';
        z -= (int) z;
    }
    * ptr = 0;

    // Connect decimal part and fraction part.
    strcat(res, r1);
    strcat(res, ".");
    strcat(res, r2);

    // Get the length of the result.
    ptr = res;
    while (* (ptr++));

    // Get only 10 digits from the result
    if (ptr - res > 10) {
        // Get the position of dot.
        ptr2 = res;
        while (* ptr2++ != '.');

        // When the dot is out of 10 digits = decimal part is over 10 digits, show error.
        if (ptr2 - res > 11) {
            lock = 1;
            return "!OVERFLOW!";
        }

        res [ 10 ] = 0;
        ptr = res + 10;
    }

    // Remove 0 in the fraction part.
    if (ptr >= ptr2) {
        while (* (--ptr) == '0' && ptr >= ptr2)
            * ptr = 0;
    }
    // If there's no digits in fraction part, remove dot.
    if (* ptr == '.')* ptr = 0;

    return res;
}

/// void num_clear()
///     description : Clear the current input number.
void num_clear() {
    dot = 0;
    inp_sig = 0;
    length = 0;
    memset(inpnum, 0, sizeof (inpnum));
    inpnum [ 0 ] = '0';
}

/// void num_init()
///     description : Clear all results and reset.
void num_init() {
    lock = 0;
    result = 0;
    cur_task = NOTHING;
    num_clear();
}

/// void memory_clear()
///     description : Clear the memorized value.
void memory_clear() {
    mem = 0;
    memory = 0;
}

/// void print_input()
///     description : Print the current input number.
void print_input() {
    mvprintw(2, 5, "%c%10s", inp_sig ? '-' : ' ', inpnum);
    cur_print = 0;
}

/// void print_result()
///     description : Print the result.
void print_result() {
    if (lock) return;
    num_clear();
    mvprintw(2, 5, "%c%10s", result >= 0 ? ' ' : '-', number_to_string(result));
    cur_print = 1;
}

/// void pre_calc()
///     description : Do calculation.
void pre_calc() {
    double r;
    switch (cur_task) {
        case NOTHING:
            result = atof(inpnum) * (inp_sig ? -1 : 1);
            break;
        case PLUS:
            result += atof(inpnum) * (inp_sig ? -1 : 1);
            break;
        case MINUS:
            result -= atof(inpnum) * (inp_sig ? -1 : 1);
            break;
        case TIME:
            result *= atof(inpnum) * (inp_sig ? -1 : 1);
            break;
        case DIVIDE:
            r = atof(inpnum) * (inp_sig ? -1 : 1);
            // If a divider is zero, show error.
            if (r == 0.0) {
                lock = 1;
                mvprintw(2, 6, "!DIVIDE 0!");
            } else {
                result /= r;
            }
            break;
        case EQUAL:
            break;
    }
}

/// void input()
///     description : Input a key and process it.
void input() {
    char c = mvgetch(18, 0);
    mvprintw(18, 0, "   ");

    // Calculation is always occur after a number not when the operator is in.
    // So do calculation when got the next operator. => pre_calc()

    // When the lock is on, stop every operation. => lock

    switch (c) {
        // A dot should be used only once.
        case '.':
            if (lock)break;
            if (dot) break;
            dot = 1;
            if (!length) ++ length;
        // Zero cannot be more than 1 in front of a number.
        case '0':
            if (lock)break;
            if (length == 0) {
                print_input();
                break;
            }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (lock)break;
            // When the last operation was = , input a number means start new calculation
            if (cur_task == EQUAL) {
                num_clear();
                cur_task = NOTHING;
            }
            // Input the number in character array within 10 digits.
            if (length < 10) {
                inpnum [ length++ ] = c;
                print_input();
            }
            break;
        // When doing calculation, finish with previous operator and print the result, and finally set current operator.
        case '+':
            if (lock) break;
            pre_calc();
            print_result();
            cur_task = PLUS;
            break;
        case '-':
            if (lock)break;
            pre_calc();
            print_result();
            cur_task = MINUS;
            break;
        case '*':
            if (lock)break;
            pre_calc();
            print_result();
            cur_task = TIME;
            break;
        case '/':
            if (lock)break;
            pre_calc();
            print_result();
            cur_task = DIVIDE;
            break;
        case '=':
        case '\n':
        case '\r':
            if (lock)break;
            pre_calc();
            print_result();
            cur_task = EQUAL;
            break;
        // Get from memory.
        case 'M':
        case 'm':
            if (lock)break;
            if (cur_task == EQUAL) {
                num_clear();
                cur_task = NOTHING;
            }
            // If the memorized value is under 0, set sign. => Input number cannot be negative.
            if (memory < 0) inp_sig = 1;
            strcpy(inpnum, number_to_string(memory));
            length = strlen(inpnum);
            print_input();
            break;
        // Remove memory.
        case 'R':
        case 'r':
            if (lock)break;
            memory_clear();
            mvprintw(2, 2, " ");
            break;
        // Add to memory.
        case 'P':
        case 'p':
            if (lock)break;
            mvprintw(2, 2, "M");
            memory += (cur_print ? result : atof(inpnum)); // Check the value is from the result or from the input
            break;
        // Subtract from memory.
        case 'Q':
        case 'q':
            if (lock)break;
            mvprintw(2, 2, "M");
            memory -= (cur_print ? result : atof(inpnum)); // Check the value is from the result or from the input
            break;
        // Clear all result.
        case 'C':
        case 'c':
            num_init();
            print_input();
            break;
        // Clear current number.
        case 'D':
        case 'd':
            if (lock)break;
            num_clear();
            print_input();
            break;
        // Exit.
        case 'X':
        case 'x':
            ext = 1;
            break;
    }
}

/// int main()
///     description : Entry point of cipher.
///     return : Returns 0 when the job is completed.
int main() {
    print_init();
    num_init();
    print_input();
    while (!ext) {
        input();
    }
    print_term();
    return 0;
}
