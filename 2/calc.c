#include <stdio.h>
#include <string.h>
#include <curses.h>

#define max 10000000000

#define NOTHING 0
#define PLUS 1
#define MINUS 2
#define TIME 3
#define DIVIDE 4

long double memory = 0.0 ;
long double result = 0.0 ;
short cur_task = 0 ;
short task = 0 ;
short comp = 0 ;
long double cur = 0.0 ;
short dot = 0 ;
short mem = 0 ;
short ext = 0 ;

void print_init ( ) {
	initscr ( ) ;
	printw ( "+====================+\n" ) ;
	printw ( "|   +------------+   |\n" ) ;
	printw ( "|   |            |   |\n" ) ;
	printw ( "|   +------------+   |\n" ) ;
	printw ( "+--------------------+\n" ) ;
	printw ( "| M R  M C  M +  M - |\tMR: M\tMC: R\tM+: P\tM-: Q\n" ) ;
	printw ( "|                    |\n" ) ;
	printw ( "| CLR  C E  +/-   X  |\tCLR: C\tCE: D\t+/-: S\tExit: X\n" ) ;
	printw ( "|                    |\n" ) ;
	printw ( "|  1    2    3    +  |\n" ) ;
	printw ( "|                    |\n" ) ;
	printw ( "|  4    5    6    -  |\n" ) ;
	printw ( "|                    |\n" ) ;
	printw ( "|  7    8    9    *  |\n" ) ;
	printw ( "|                    |\n" ) ;
	printw ( "|  0    .    =    /  |\n" ) ;
	printw ( "+====================+\n" ) ;
}

void print_term ( ) {
	mvprintw ( 18 , 0 , "Thank you for using Calculator!! \n" ) ;
	endwin ( ) ;
}

char * strrev ( char * str ) {
	char * f = str - 1 , * t = str ;

	while ( * t ) ++ t ;

	while ( ++ f < -- t )
		* f ^= * t ^= * f ^= * t ;

	return str ;
}

char * number_to_string ( long double number ) {
	if ( number < 0 ) number = -number ;
	number += 0.000000005 ;

	char res [ 15 ] ;
	char r1 [ 11 ] ;
	char r2 [ 11 ] ;
	long long r = number ;
	long double z = number - r ;
	char * ptr ;

	// Change integer to char array r1.
	ptr = r1 ;

	while ( r != 0 ) {
		* ptr ++ = r % 10 + '0' ;
		r /= 10 ;
	}
	if ( ptr == r1 ) {
		* ptr ++ = '0' ;
	}
	* ptr = 0;

	strrev ( r1 ) ;

	// Change double to char array r2
	ptr = r2 ;

	while ( ( ptr - r2 ) < 8 ) {
		z *= 10 ;
		* ptr ++ = (int)z + '0';
		z -= (int)z ;
	}
	* ptr = 0 ;

	strcat ( res , r1 ) ;
	strcat ( res , "." ) ;
	strcat ( res , r2 ) ;

	ptr = res ;
	while ( * ( ptr ++ ) ) ;

	if ( ptr - res > 10 ) {
		res [ 10 ] = 0 ;
		ptr = res + 10 ;
	}

	while ( * ( -- ptr ) == '0' && ptr >= res )
		* ptr = 0 ;
	++ ptr ;

	if ( * ( ptr - 1 ) == '.' ) * ptr = 0 ;

	return res ;
}

void print_number ( ) {
	char t [ 100 ] ;
	if ( cur >= max ) {
		mvprintw ( 2 , 6 , "!OVERFLOW!" ) ;
	}
	else {
		if ( cur >= 0 ) {
			mvprintw ( 2 , 5 , " " ) ;
		}
		else {
			mvprintw ( 2 , 5 , "-" ) ;
		}
		mvprintw ( 2 , 6 , "%10s" , number_to_string(cur) ) ;
	}
}

void input ( ) {
	char c = mvgetch ( 18 , 0 ) ;
	mvprintw ( 18 , 0 , " " ) ;
	switch ( c ) {
		case '0' :
		case '1' :
		case '2' :
		case '3' :
		case '4' :
		case '5' :
		case '6' :
		case '7' :
		case '8' :
		case '9' :
			cur = ( cur * 10 + c - '0' ) / ( dot ? 10 : 1 ) ;
			print_number ( ) ;
			break ;
		case '.' :
			dot = 1 ;
			break ;
		case '+' :
			result += cur ;
			cur = result ;
			task = 1 ;
			cur_task = PLUS ;
			dot = 0 ;
			break ;
		case '-' :
			result -= cur ;
			cur = result ;
			task = 1 ;
			cur_task = MINUS ;
			break ;
		case '*' :
			result *= cur ;
			cur = result ;
			task = 1 ;
			cur_task = TIME ;
			break ;
		case '/' :
			result /= cur ;
			cur = result ;
			task = 1 ;
			cur_task = DIVIDE ;
			break ;
		case '=' :
			switch ( cur_task ) {
				case PLUS :
					result += cur ;
					break ;
				case MINUS :
					result -= cur ;
					break ;
				case TIME :
					result *= cur ;
					break ;
				case DIVIDE :
					result /= cur ;
					break ;
				default :
					result = cur ;
					break ;
			}
			cur_task = NOTHING ;
			cur = result ;
			task = 1 ;
			comp = 1 ;
			break ;
		case 'M' :
		case 'm' :
			cur = memory ;
			break ;
		case 'R' :
		case 'r' :
			memory = 0 ;
			mvprintw ( 2 , 3 , " " ) ;
			break ;
		case 'P' :
		case 'p' :
			mvprintw ( 2 , 3 , "M" ) ;
			memory += cur ;
			break ;
		case 'Q' :
		case 'q' :
			mvprintw ( 2 , 3 , "M" ) ;
			memory -= cur ;
			break ;
		case 'C' :
		case 'c' :
			cur = result = 0.0 ;
			break ;
		case 'D' :
		case 'd' :
			cur = 0.0 ;
			break ;
		case 'S' :
		case 's' :
			cur = - cur ;
			break ;
		case 'X' :
		case 'x' :
			ext = 1 ;
			break ;
		default :
			break ;
	}
}

int main ( ) {
	print_init ( ) ;
	while ( ! ext ) {
		input ( ) ;
	}
	print_term ( ) ;
	return 0 ;
}
