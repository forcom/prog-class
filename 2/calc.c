#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <memory.h>

#define max 10000000000

#define NOTHING 0
#define PLUS 1
#define MINUS 2
#define TIME 3
#define DIVIDE 4
#define EQUAL 5

void num_clear();
void num_init();

long double memory = 0.0;
long double result = 0.0;
short cur_task = 0;
short cur_print = 0;
short dot = 0;
short mem = 0;
short ext = 0;
short lock = 0;

char inpnum [ 12 ];
int length;

void print_init() {
    initscr();
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

void print_term() {
    mvprintw(18, 1, "Thank you for using Calculator!! \n");
    endwin();
}

char * strrev(char * str) {
    char * f = str - 1, * t = str;

    while (* t) ++ t;

    while (++f < --t)
        * f ^= *t ^= *f ^= *t;

    return str;
}

char * number_to_string(long double number) {
    if (number < 0) number = -number;
    number += 0.000000005;

    char res [ 15 ];
    char r1 [ 11 ];
    char r2 [ 11 ];
    long long r = number;
    long double z = number - r;
    char * ptr, *ptr2;

    res [0] = 0;

    // Change integer to char array r1.
    ptr = r1;

    while (r != 0) {
        * ptr++ = r % 10 + '0';
        r /= 10;
    }
    if (ptr == r1) {
        * ptr++ = '0';
    }
    * ptr = 0;

    strrev(r1);

    // Change double to char array r2
    ptr = r2;

    while ((ptr - r2) < 8) {
        z *= 10;
        * ptr++ = (int) z + '0';
        z -= (int) z;
    }
    * ptr = 0;

    strcat(res, r1);
    strcat(res, ".");
    strcat(res, r2);

    ptr = res;
    while (* (ptr++));

    if (ptr - res > 10) {
        ptr2 = res;
        while (* ptr2++ != '.');
        if (ptr2 - res > 11) {
            num_init();
            return "!OVERFLOW!";
        }
        res [ 10 ] = 0;
        ptr = res + 10;
    }

    if (ptr >= ptr2) {
        while (* (--ptr) == '0' && ptr >= ptr2)
            * ptr = 0;
    }
    if (* ptr == '.')* ptr = 0;

    return res;
}

void num_clear() {
    dot = 0;

    length = 0;
    memset(inpnum, 0, sizeof ( inpnum));
    inpnum [ 0 ] = '0';
}

void num_init() {
    result = 0;
    num_clear();
}

void memory_clear() {
    mem = 0;
    memory = 0;
}

void print_input() {
    mvprintw(2, 5, " %10s", inpnum);
    cur_print = 0;
}

void print_result() {
    num_clear();
    mvprintw(2, 5, "%c%10s", result >= 0 ? ' ' : '-', number_to_string(result));
    cur_print = 1;
}

void pre_calc() {
    double r;
    switch (cur_task) {
        case NOTHING:
            result = atof(inpnum);
            break;
        case PLUS:
            result += atof(inpnum);
            break;
        case MINUS:
            result -= atof(inpnum);
            break;
        case TIME:
            result *= atof(inpnum);
            break;
        case DIVIDE:
            r = atof(inpnum);
            if (r == 0.0) {
                num_init();
                mvprintw(2, 6, "!DIVIDE 0!");
            } else {
                result /= atof(inpnum);
            }
            break;
        case EQUAL:
            break;
    }
}

void input() {
    char c = mvgetch(18, 0);
    mvprintw(18, 0, " ");

    switch (c) {
        case '.':
            if (lock)break;
            if (dot) break;
            dot = 1;
            if (!length) ++ length;
        case '0':
            if (lock)break;
            if (length == 0) break;
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
            if (cur_task == EQUAL) {
                num_clear();
                cur_task = NOTHING;
            }
            if (length < 10) {
                inpnum [ length++ ] = c;
                print_input();
            }
            break;
        case '+':
            pre_calc();
            print_result();
            cur_task = PLUS;
            lock = 0;
            break;
        case '-':
            pre_calc();
            print_result();
            cur_task = MINUS;
            lock = 0;
            break;
        case '*':
            pre_calc();
            print_result();
            cur_task = TIME;
            lock = 0;
            break;
        case '/':
            pre_calc();
            print_result();
            cur_task = DIVIDE;
            lock = 0;
            break;
        case '=':
        case '\n':
        case '\r':
            pre_calc();
            print_result();
            cur_task = EQUAL;
            lock = 0;
            break;
        case 'M':
        case 'm':
            if (cur_task == EQUAL) {
                num_clear();
                cur_task = NOTHING;
            }
            strcpy(inpnum, number_to_string(memory));
            length = strlen(inpnum);
            print_input();
            break;
        case 'R':
        case 'r':
            memory_clear();
            mvprintw(2, 2, " ");
            break;
        case 'P':
        case 'p':
            mvprintw(2, 2, "M");
            memory += (cur_print ? result : atof(inpnum));
            break;
        case 'Q':
        case 'q':
            mvprintw(2, 2, "M");
            memory -= (cur_print ? result : atof(inpnum));
            break;
        case 'C':
        case 'c':
            num_init();
            print_input();
            break;
        case 'D':
        case 'd':
            num_clear();
            print_input();
            break;
        case 'X':
        case 'x':
            ext = 1;
            break;
    }
}

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
