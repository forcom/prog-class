///
///  cal.c
///      Shows a calendar
///
///  Compile :
///      Compile with a standard C compiler.
///
///      $ cc -o cal -lm cal.c
///
///  Usage :
///
///      Execute ./cal and input a year and a date to show.
///
///  Written by Yoon, SeungYong, 2012. 06. 22.
///
#include <stdio.h>

int year ; //year input
int month ; //month input

// days in each months
int dates [ 13 ] = { 0 , 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;

// calendar to print
int cal [ 6 ] [ 7 ] ;
int posx , posy ; // current position of cal

/// void input ( )
///     description : Input a year and a date
void input ( ) {
    printf ( "年を入力してください: " ) ;
    scanf ( "%d" , & year ) ;
    printf ( "月を入力してください: " ) ;
    scanf ( "%d" , & month ) ;
}

/// short is_leap ( int year )
///     description : Check if the year is the leap year
///     argument :
///         year : an integer represents a year to check
///     result : Returns 1 when the year is the leap year, otherwise 0.
short is_leap ( int year ) {
    // When the year is multiples of 400, or multiples of 4 and not 100, it is the leap year.
    return ! ( year % 400 ) || ( ! ( year % 4 ) && year % 100 ) ;
}

/// void process ( )
///     description : Make a calendar
void process ( ) {
    int i , days = 0 ;

    // If the input is the leap year, add a day in Feburary.
    if ( is_leap ( year ) ) {
        ++ dates [ 2 ] ;
    }

    // In 1967, January 1st is Sunday.

    // If the input is over 1967,
    if ( year >= 1967 ) {
        // Add days from 1967 to input year
        for ( i = 1967 ; i < year ; ++ i ) {
            days += 365 + is_leap ( i ) ;
        }
        // ... before a month
        for ( i = 1 ; i < month ; ++ i ) {
            days += dates [ i ] ;
        }

        // Find a day of the week.
        posx = days % 7 ;
    }
    // If the input is under 1967,
    else {
        // Subtract days from 1966 to input year
        for ( i = 1966 ; i >= year ; -- i ) {
            days -= 365 + is_leap ( i ) ;
        }
        // ... before a month
        for ( i = 1 ; i < month ; ++ i ) {
            days += dates [ i ] ;
        }

        // Find a days of the week.
        posx = ( ( days % 7 ) + 7 ) % 7 ;
    }

    // Make a calendar!    
    for ( i = 1 ; i <= dates [ month ] ; ++ i ) { // i is a day.
        cal [ posy ] [ posx ] = i ;
        posx = ( posx + 1 ) % 7 ; // a day of the week
        if ( ! posx ) ++ posy ; // the number of the week in the month.
    }
}

/// void output ( )
///     description : Print a calendar.
void output ( ) {
    int i , j ;
    printf ( "\n     %4d年 %2d月\n" , year , month ) ;
    printf ( "日 月 火 水 木 金 土\n" ) ;
    for ( i = 0 ; i <= posy ; ++ i ) {
        for ( j = 0 ; j < 7 ; ++ j ) {
            // If there's a day, print, otherwise do not print.
            if ( cal [ i ] [ j ] ) {
                printf ( "%2d " , cal [ i ] [ j ] ) ;
            }
            else {
                printf ( "   " ) ;
            }
        }
        printf ( "\n" ) ;
    }
}

/// int main ( int argc, char ** argv )
///     description : Entry point of cipher.
///     return : Returns 0 when the job is succeed.
int main ( ) {
    input ( ) ;
    process ( ) ;
    output ( ) ;
    return 0 ;
}
