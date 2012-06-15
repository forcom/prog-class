#include <stdio.h>

short exit = 0;

void print_command ( ) {
	printf ( "+-*/()= MR[M] MC[R] CLR[C] CE[D]\n>> " ) ;
}

void input ( ) {
}

void process ( ) {
}

void output ( ) {
}

int main ( ) {
	while ( ! exit ) {
		print_command ( ) ;
		input ( ) ;
		process ( ) ;
		output ( ) ;
	}
	return 0 ;
}
