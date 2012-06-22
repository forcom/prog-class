#include <stdio.h>

int year ;
int month ;

const int def_date = 1967 ;
const int def_day = 0 ;
int dates [ 13 ] = { 0 , 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;

int cal [ 6 ] [ 7 ] ;
int posx , posy ;

void input ( ) {
    printf ( "年を入力してください: " ) ;
    scanf ( "%d" , & year ) ;
    printf ( "月を入力してください: " ) ;
    scanf ( "%d" , & month ) ;
}

short is_leap ( int year ) {
    return ! ( year % 400 ) || ( ! ( year % 4 ) && year % 100 ) ;
}

void process ( ) {
    int i , days = 0 ;

    if ( is_leap ( year ) ) {
        ++ dates [ 2 ] ;
    }

    if ( year >= 1967 ) {
        for ( i = 1967 ; i < year ; ++ i ) {
            days += 365 + is_leap ( i ) ;
        }
        for ( i = 1 ; i < month ; ++ i ) {
            days += dates [ i ] ;
        }

        posx = days % 7 ;
    }
    else {
        for ( i = 1967 ; i >= year ; -- i ) {
            days -= 365 + is_leap ( i ) ;
        }
        days ++ ;

        for ( i = 1 ; i < month ; ++ i ) {
            days += dates [ i ] ;
        }

        posx = ( ( days % 7 ) + 7 ) % 7 ;
    }
    
    for ( i = 1 ; i <= dates [ month ] ; ++ i ) {
        cal [ posy ] [ posx ] = i ;
        posx = ( posx + 1 ) % 7 ;
        if ( ! posx ) ++ posy ;
    }
}

void output ( ) {
    int i , j ;
    printf ( "\n     %4d年 %2d月\n" , year , month ) ;
    printf ( "日 月 火 水 木 金 土\n" ) ;
    for ( i = 0 ; i <= posy ; ++ i ) {
        for ( j = 0 ; j < 7 ; ++ j ) {
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

int main ( ) {
    input ( ) ;
    process ( ) ;
    output ( ) ;
    return 0 ;
}
