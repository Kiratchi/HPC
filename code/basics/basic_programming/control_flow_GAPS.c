#include <stdio.h>

int
main(
    int argc,
    char *argv[]
    )
{
  int a;

  for ( a = 0; a < 5; ++a ) {
    if /*INSERT*/ {
      printf("a == 0\n");
    }

    if ( a == 1 ) {
      printf("a == 1\n");
    /*INSERT*/
      printf("a != 1\n");
    }

    if ( a == 1 ) {
      printf("a == 1\n");
    } else if ( a == 2 ) {
      printf("a == 2\n");
    } else {
      printf("a != 1 && a != 2\n");
    }
  }

  printf("a == %i\n", a);


  for ( a = 0; a < 5; /*INSERT*/ ) {
    if ( a == 2 )
      break;
    printf("a == %i\n", a);
  }

  printf("final: a == %i\n", a);


  for ( /*INSERT*/; /*INSERT*/; ++a ) {
    if ( a == 2 )
      continue;
    printf("a == %i\n", a);
  }

  printf("final: a == %i\n", a);


  return 0;
}
