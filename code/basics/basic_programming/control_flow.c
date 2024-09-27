/*
 * - An overview of all C keywords including those for controle flow is
 *   available at
 *   https://en.cppreference.com/w/c/keyword
 * - if statements are explained at
 *   https://en.cppreference.com/w/c/language/if
 * - notice that "else if" is composed of two keywords "else" and "if", but
 *   there is no special keywords for this.
 * - for loops are explained at
 *   https://en.cppreference.com/w/c/language/for
 * - break is explained at
 *   https://en.cppreference.com/w/c/language/break
 * - continue is explained at
 *   https://en.cppreference.com/w/c/language/continue
 */
#include <stdio.h>

int
main(
    int argc,
    char *argv[]
    )
{
  int a;

  for ( a = 0; a < 5; ++a ) {
    if ( a == 0 ) {
      printf("a == 0\n");
    }

    if ( a == 1 ) {
      printf("a == 1\n");
    } else {
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


  for ( a = 0; a < 5; ++a ) {
    if ( a == 2 )
      break;
    printf("a == %i\n", a);
  }

  printf("final: a == %i\n", a);


  for ( a = 0; a < 5; ++a ) {
    if ( a == 2 )
      continue;
    printf("a == %i\n", a);
  }

  printf("final: a == %i\n", a);


  return 0;
}
