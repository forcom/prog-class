///
///  cipher.c
///      Cipher/decipher a string with a key.
///      This code uses the application of Caesar cipher.
///
///  Compile :
///      Compile with a standard C compiler.
///
///  Usage :
///      cipher  e|d  key  string
///
///       e : Encrypt a string with a key, and print with standard output
///       d : Decrypt a string with a key, and print with standard output
///       key : A natural number less than INT_MAX. Be sure a key is not divided with 26.
///       string : An alphabet only string.
///
///  Written by Yoon, SeungYong, 2012. 06. 07.
///
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// short is_capital ( char c )
///     description : Check a character is a capital letter
///     argument :
///         c : An alphabet character
///     result : Returns 1 when the character is a capital letter, otherwise returns 0.
short is_capital ( char c ) {
	return 'A' <= c && c <= 'Z' ;
}

/// short is_num ( char * str )
///     description : Check a string consists of only numbers.
///     argument :
///         str : A char string
///     result : Returns 1 when the string consists of only numbers, otherwise returns 0.
short is_num ( char * str ) {
	while ( * str ) {
		if ( * str < '0' || * str > '9' ) return 0 ;
		++ str ;
	}
	return 1 ;
}

/// int get_key ( char * str )
///     description : Get an integer number from a string
///     argument :
///         str : A char string
///     result : Returns an integer number from a string.
///     caution : Be sure to check a string consists with only numbers.
int get_key ( char * str ) {
	return atoi ( str ) ;
}

/// char * encode ( int key, char * data )
///     description : Get a ciphered text with a key from a plaintext string
///     argument :
///         key : A natural number to use in encryption
///         data : A plaintext string to cipher. Be sure not to have any non-alphabet character.
///     result : Returns a ciphered string.
char * encode ( int key, char * data ) {
	int length = strlen ( data ) ;
        char * result = malloc ( sizeof ( char ) * length ) ; // Assigning a result array.
	char * dp = data , * rp = result ; // Sets a character pointer of the plaintext and the result 

	key %= 26 ;

	while ( * dp ) {
		// When a plain-character is a uppercase, lowersize it and shift it by the key
		if ( is_capital ( * dp ) ) {
			* rp = ( * dp - 'A' + key ) % 26 + 'a' ;
		}
		// When a plain-character is a lowercase, uppersize it and shift it by the key
		else {
			* rp = ( * dp - 'a' + key ) % 26 + 'A' ;
		}
		++ dp ;
		++ rp ;
	}
	* rp = 0 ;
	return result ;
}

/// char * decode ( int key, char * data )
///     description : Get a deciphered text with a key from a ciphered text string
///     argument :
///         key : A natural number to use in decryption
///         data : A ciphered string to decipher. Be sure not to have any non-alphabet character.
///     result : Returns a deciphered string.
char * decode ( int key, char * data ) {
	int length = strlen ( data ) ;
	char * result = malloc ( sizeof ( char ) * length ) ; // Assigning a result array.
	char * dp = data , * rp = result ; // Sets a character pointer of the plaintext and the result

	key %= 26 ;

	while ( * dp ) {
		// When a plain-character is a uppercase, lowersize it and shift back it by the key
		if ( is_capital ( * dp ) ) {
			* rp = ( * dp - 'A' + 26 - key ) % 26 + 'a' ;
		}
		// When a plain-character is a lowercase, uppersize it and shift back it by the key
		else {
			* rp = ( * dp - 'a' + 26 - key ) % 26 + 'A' ;
		}
		++ dp ;
		++ rp ;
	}
	* rp = 0 ;
	return result ;
}

/// void print_usage ( void )
///     description : Print a usage of cipher with standard output
void print_usage ( ) {
	printf ( "Usage : cipher e|d key string\n\n e : Encode a string\n d : Decode a string\n key : a number to cipher a string\n" ) ;
}

/// int main ( int argc, char ** argv )
///     description : Entry point of cipher.
///     argument : as expressed in cipher usage.
///     return : Returns 0 when the job is succeed, otherwise returns non-zero.
int main ( int argc , char ** argv ) {
	// Checks if the arguments are correctly passed.
	if ( argc != 4 ) {
		print_usage ( ) ;
		return 1 ;
	}

	// Checks if a key is a number
	if ( ! is_num ( * ( argv + 2 ) ) ) {
		print_usage ( ) ;
		return 1 ;
	}

	// If the option is 'e', do encryption.
	if ( ** ( argv + 1 ) == 'e' ) {
		printf ( "%s\n" , encode ( get_key ( * ( argv + 2 ) ) , * ( argv + 3 ) ) ) ;
	}
	// If the option is 'd', do decryption.
	else if ( ** ( argv + 1 ) == 'd' ) {
		printf ( "%s\n" , decode ( get_key ( * ( argv + 2 ) ) , * ( argv + 3 ) ) ) ;
	}
	// Otherwise, print error.
	else {
		print_usage ( ) ;
		return 1 ;
	}

	return 0 ;
}
