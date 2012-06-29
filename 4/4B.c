///
///  4B.c
///      abcde * fghij = ABCDEFGHIJ
///      a-j are a digit 0-9 and all different
///      A-J are a digit 0-9 and all different
///
///      Finding these numbers.
///
///  Compile :
///      Compile with a standard C compiler.
///
///      $ cc -o 4B -lm 4B.c
///
///  Written by Yoon, SeungYong, 2012. 06. 29.
///
#include <stdio.h>
#include <memory.h>

/// swap( a, b )
///     description : Swap two variables.
#define swap(a,b) a^=b^=a^=b

int num [ 10 ] = { 1 , 0 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 } ; // for permutation

/// short reverse ( int x , int y )
///     description : Reverse the permutation between x and y
///     argument :
///         x : Index of the first element.
void reverse ( int x ) {
    int y = 9 ;
    while ( x < y )
        swap ( num [ x ++ ] , num [ y -- ] ) ;
}

/// short next_permutation ( )
///     description : Get the next permutation
///     result : Returns 1 when there is a next permutation, otherwise 0.
short next_permutation ( ) {
    int first = 0 , last = 10 ;
    int i = last - 1 ;

    while (1) {
        int i1, i2;

        i1 = i;
        if ( num [ --i ] < num [ i1 ] ) {
            i2 = last;
            while ( !( num [ i ] < num [ --i2 ] ) );
            swap ( num [ i ] , num [ i2 ] ) ;
            reverse ( i1 ) ;
            return 1;
        }
        if (i == first) {
            reverse ( first ) ;
            return 0 ;
        }
    }
}

/// short check ( long long x )
///     description : Check if the number has all digits.
///     argument :
///         x : A number to check.
///     result : Returns 1 when the number has all digits, otherwise 0.
short check ( long long x ) {
    short check [ 10 ] ;
    memset ( check , 0 , sizeof ( check ) ) ;
    while ( x ) {
        check [ x % 10 ] = 1 ;
        x /= 10 ;
    }
    for ( int i = 0 ; i < 10 ; ++ i ) {
        if ( ! check [ i ] ) return 0 ;
    }
    return 1 ;
}

/// int main ( int argc, char ** argv )
///     description : Entry point of cipher.
///     return : Returns 0 when the job is succeed.
int main( ) {
    long long x , y , z ;
    int i , cnt = 0 ;
    do {
        x = 0 ;
        y = 0 ;
        z = 0 ;
        for ( i = 0 ; i < 5 ; ++ i ) {
            x = x * 10 + num [ i ] ;
            y = y * 10 + num [ i + 5 ] ;
        }
        z = x * y ;
        if ( x < 10000 || y < 10000 || z < 1000000000 || ! check ( z ) ) continue ;
        printf ( "%lld * %lld = %lld\n" , x , y , z ) ;
        ++ cnt ;
    } while ( next_permutation ( num , num + 10 ) ) ;
    printf ( "Count : %d\n" , cnt ) ;
    return 0;
}
