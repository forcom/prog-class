#include <stdio.h>
#include <stdlib.h>

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
short sig = 0 ;
short mem = 0 ;
short ext = 0 ;

void move_digit ( ) {
	printf ( "\x1b[3;7H" ) ;
}

void move_signal ( ) {
	printf ( "\x1b[3;6H" ) ;
}

void move_memory ( ) {
	printf ( "\x1b[3;3H" ) ;
}

void print_init ( ) {
	printf ( "\x1b[2J\x1b[0;0H" ) ;
	puts ( "+====================+" ) ;
	puts ( "|   +------------+   |" ) ;
	puts ( "|   | 0123456789 |   |" ) ;
	puts ( "|   +------------+   |" ) ;
	puts ( "+--------------------+" ) ;
	puts ( "| M R  M C  M +  M - |\tMR: M\tMC: R\tM+: P\tM-: Q" ) ;
	puts ( "|                    |" ) ;
	puts ( "| CLR  C E  +/-   X  |\tCLR: C\tCE: D\t+/-: S\tExit: X" ) ;
	puts ( "|                    |" ) ;
	puts ( "|  1    2    3    +  |" ) ;
	puts ( "|                    |" ) ;
	puts ( "|  4    5    6    -  |" ) ;
	puts ( "|                    |" ) ;
	puts ( "|  7    8    9    *  |" ) ;
	puts ( "|                    |" ) ;
	puts ( "|  0    .    =    /  |" ) ;
	puts ( "+====================+\n" ) ;
}

char * dtoa ( long double number ) {
	if ( number < 0 ) {
		sig = 1 ;
		number *= - 1 ;
	}

	char res [ 11 ] ;
	long long r = number ;
	long double z = number - r ;
	char * ptr = res , * f = res , * t ;

	while ( r != 0 ) {
		* ptr ++ = r % 10 + '0' ;
		r /= 10 ;
	}
	* ptr = 0;

	t = ptr - 1 ;
	while ( f < t ) {
		* f ^= * t ^= * f ^= * t ;
		++ f ;
		-- t ;
	}

	if ( ptr == res ) {
		* ptr ++ = '0' ;
		* ptr = 0 ;
	}

	if ( z == 0.0 ) return res ;
	* ptr ++ = '.' ;

	if ( ptr - res == 10 ) { 
		* ( -- ptr ) = 0 ;
		return res ;
	}

	while ( ( ptr - res ) < 10 && z != 0.0 ) {
		z *= 10 ;
		* ptr ++ = (int)z + '0';
		z -= (int)z ;
	}
	* ptr = 0 ;
	-- ptr ;
	while ( * ptr == '0' ) {
		* ptr = 0 ;
		-- ptr ;
	}

	return res ;
}

void print_number ( ) {
	char t [ 100 ] ;
	move_digit ( ) ;
	if ( cur >= max ) {
		printf ( "!OVERFLOW!" ) ;
	}
	else {
		printf ( "%10s\n" , dtoa(cur) ) ;
	}

	move_signal ( ) ;
	printf ( "%c" , sig ? '-' : ' ' ) ;

	printf ( "\x1b[18;0H" ) ;
}

void input ( ) {
	fflush ( stdin ) ;
	char c = getchar ( ) ;
	if ( task ) {
		cur = 0 ;
		task = 0 ;
		sig = 0 ;
	}
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
			break ;
		case '.' :
			dot = 1 ;
			break ;
		case '+' :
			result += cur * ( sig ? -1 : 1 ) ;
			cur = result ;
			task = 1 ;
			cur_task = PLUS ;
			break ;
		case '-' :
			result -= cur * ( sig ? -1 : 1 ) ;
			cur = result ;
			task = 1 ;
			cur_task = MINUS ;
			break ;
		case '*' :
			result *= cur * ( sig ? -1 : 1 ) ;
			cur = result ;
			task = 1 ;
			cur_task = TIME ;
			break ;
		case '/' :
			result /= cur * ( sig ? -1 : 1 ) ;
			cur = result ;
			task = 1 ;
			cur_task = DIVIDE ;
			break ;
		case '=' :
			switch ( cur_task ) {
				case PLUS :
					result += cur * ( sig ? -1 : 1 ) ;
					break ;
				case MINUS :
					result -= cur * ( sig ? -1 : 1 ) ;
					break ;
				case TIME :
					result *= cur * ( sig ? -1 : 1 ) ;
					break ;
				case DIVIDE :
					result /= cur * ( sig ? -1 : 1 ) ;
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
			move_memory ( ) ;
			printf ( " " ) ;
			break ;
		case 'P' :
		case 'p' :
			move_memory ( ) ;
			printf ( "M" ) ;
			memory += cur ;
			break ;
		case 'Q' :
		case 'q' :
			move_memory ( ) ;
			printf ( "M" ) ;
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
			sig ^= 1 ;
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
		print_number ( ) ;
		input ( ) ;
	}
	return 0 ;
}
