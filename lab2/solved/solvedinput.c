// This is a single line comment
/* *****This is a
******Multiline Comment
**** */
#include <stdio.h>
void main()
{
FILE *fopen(), *fp;int c ;
fp = fopen( “prog.c”, “r” ); //Comment
c = getc( fp ) ;
while ( c
!=
EOF )
{
putchar( c );
c = getc ( fp );
}
/*multiline
comment */
fclose(
fp );
}


// #include <stdio.h>
// #include<limits.h>
// #define MAX 100

// int main() {
//     printf("Hello, World!\n");
//     return 0;
// }