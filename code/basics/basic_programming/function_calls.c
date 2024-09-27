/*
 * - Functions are documented at
 *   https://en.cppreference.com/w/c/language/functions
 * - Calling functions, strictly speaking, is an operator, which explained at
 *   https://en.cppreference.com/w/c/language/operator_other#Function_call
 */

#include <stdio.h>

// You may give any argument name, for instance,
// int printf_from_snd_argument(int argc_alt, char *argv_alt[]);

void printf_from_snd_argument(int, char*[]);

int
main(
    int argc,
    char *argv[]
    )
{
  printf("command line arguments\n");

  printf_from_snd_argument(argc, argv);

  return 0;
}

void
printf_from_snd_argument(
    int argc,
    char *argv[]
    )
{
  for ( int ix = 1; ix < argc; ++ix ) {
    printf("%s\n", argv[ix]);
  }
}
