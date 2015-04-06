#include <stdio.h>
#include <stdlib.h>


int main( int argc, char *argv[] )
{
        if( argc != 2 )
        {
                printf("Error - supply a format string please\n");
                return 1;
        }
        printf( argv[1] );
        printf( "\n" );

        return 0;

}
