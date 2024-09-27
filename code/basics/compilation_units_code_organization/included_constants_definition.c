/*
 * - Include statements and the preprocessor in general are described at
 *   https://en.cppreference.com/w/c/preprocessor/include
 *   https://en.cppreference.com/w/c/preprocessor
 */

#include <stdio.h>
#include "included_constants_definition.h"

int
main(
    int argc,
    char *argv[]
    )
{

  printf("a = %i, b = %i, c = %i\n", A, B, C);

  return 0;
}
