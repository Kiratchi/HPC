/* References
 * 
 * https://en.cppreference.com/w/c
 *
 * - main function
 *   https://en.cppreference.com/w/c/language/main_function
 * - printf and string formating
 *   https://en.cppreference.com/w/c/io/fprintf
 * - for loops
 *   https://en.cppreference.com/w/c/language/for
 * - include
 *   https://en.cppreference.com/w/c/preprocessor/include
 * - one-line comments are prefaced by //
 * - multi-line and inline comments are enclosed as this comment; it is not
 *   possible to nest them.
 */

#include <stdio.h>

int
main(
    int argc,
    char *argv[]
    )
{
  printf("command line arguments\n");
  
  for ( int ix = 0; ix < argc; ++ix ) {
    printf("%s\n", argv[ix]);
  }

  return 0;
}
