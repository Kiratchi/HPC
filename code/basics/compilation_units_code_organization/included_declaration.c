/*
 * - Include statements and the preprocessor in general are described at
 *   https://en.cppreference.com/w/c/preprocessor/include
 *   https://en.cppreference.com/w/c/preprocessor
 */

#include <stdio.h>
#include "included_declaration.h"

int
main(
    int argc,
    char *argv[]
    )
{
  double a = rand_double();
  double b = rand_double();
  double c = rand_double();

  printf("a = %02.4f, b = %03.5f, c = %03.1f\n", a, b, c);

  return 0;
}
