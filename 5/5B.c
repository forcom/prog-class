///
///  5B.c
///      Insert operators +, -, * between 123456789, and make 100.
///
///      Finding all these equations.
///
///  Compile :
///      Compile with a standard C compiler.
///
///      $ cc -o 5B -lm 5B.c
///
///  Written by Yoon, SeungYong, 2012. 07. 01.
///
#include <stdio.h>

int que [ 20 ] , cur ; // Expression queue , and position
int cnt ;

#define PLUS -21
#define MINUS -22
#define MULTI -31

/// int polish_notation ( int expression [ ] , int _size )
///     description : Returns the result of the expression.
///     argument :
///         expression : An expression to evaluate.
///         _size : The number of terms of the expression.
///     result : Returns the result of the expression.
int polish_notation ( int expression [ ] , int _size ) {
	int head = -1 , tail = 0 ;
	int outque [ 40 ] ; // Output Queue
	int top = 0 ;
	int operstk [ 40 ] ; // Operator Stack
	int calstk [ 40 ] ; // Calculation Stack
	int i ;

	// Reverse Polish Notation
	for ( i = 0 ; i < _size ; ++ i ) {
		if ( expression [ i ] > 0 ) {
			outque [ tail ++ ] = expression [ i ] ;
		}
		else {
			while ( top && operstk [ top - 1 ] / 10 <= expression [ i ] / 10 ) {
				outque [ tail ++ ] = operstk [ -- top ] ;
			}
			operstk [ top ++ ] = expression [ i ] ;
		}
	}
	while ( top ) {
		outque [ tail ++ ] = operstk [ -- top ] ;
	}

	// Calculate with Reverse Polish Notation.
	top = 0 ;
	while ( ++ head < tail ) {
		switch ( outque [ head ] ) {
		case PLUS :
			calstk [ top - 2 ] = calstk [ top - 2 ] + calstk [ top - 1 ] ;
			-- top ;
			break ;
		case MINUS :
			if ( top > 1 ) { // binary operator -.
				calstk [ top - 2 ] = calstk [ top - 2 ] - calstk [ top - 1 ] ;
				-- top ;
			}
			else { // unary operator -.
				calstk [ top - 1 ] = - calstk [ top - 1 ] ;
			}
			break ;
		case MULTI :
			calstk [ top - 2 ] = calstk [ top - 2 ] * calstk [ top - 1 ] ;
			-- top ;
			break ;
		default :
			calstk [ top ++ ] = outque [ head ] ;
			break ;
		}
	}

	return calstk [ top - 1 ] ;
}

/// void make_expression ( int lev )
///     description : Make the expression.
///     argument :
///         lev : Current digit to make.
void make_expression ( int lev ) {
	int i ;
	if ( lev == 10 ) {
		int res = polish_notation ( que , cur + 1 ) ;

		if ( res == 100 ) {
			for ( i = 0 ; i <= cur ; ++ i ) {
				switch ( que [ i ] ) {
				case PLUS :
					printf ( "+ " ) ;
					break ;
				case MINUS :
					printf ( "- " ) ;
					break ;
				case MULTI :
					printf ( "* " ) ;
					break ;
				default :
					printf ( "%d " , que [ i ] ) ;
					break ;
				}
			}
			printf ( "= %d\n" , res ) ;
			++ cnt ;
		}
		return ;
	}

	// No Operator
	que [ cur ] = que [ cur ] * 10 + lev ;
	make_expression ( lev + 1 ) ;
	que [ cur ] /= 10 ;

	if ( que [ cur ] ) ++ cur ;
	
	// If previous one is not an operator
	if ( cur && que [ cur - 1 ] > 0 ) {
		// Add PLUS
		que [ cur ++ ] = PLUS ;
		make_expression ( lev ) ;
		que [ -- cur ] = 0 ;

		// Add MULTI
		que [ cur ++ ] = MULTI ;
		make_expression ( lev ) ;
		que [ -- cur ] = 0 ;
	}

	// Add MINUS
	// But the first one can be negative.
	if ( ! cur || ( cur && que [ cur - 1 ] > 0 ) ) {
		que [ cur ++ ] = MINUS ;
		make_expression ( lev ) ;
		que [ -- cur ] = 0 ;
		if ( cur && que [ cur - 1 ] ) -- cur ;
	}
}

/// int main ( )
///     description : Entry point of cipher.
///     result : Returns 0 when the job is succeed.
int main ( ) {
	make_expression ( 1 ) ;
	printf ( " Count : %d\n" , cnt ) ;
	return 0 ;
}
