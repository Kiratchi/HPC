/*
 * - Fundamental types are descriped at
 *   https://en.cppreference.com/w/c/language/type
 */

#include <stdio.h>

int
main(
    int argc,
    char *argv[]
    )
{
  // The size (i.e., number of bytes) occupied by a variable of given type.
  printf("size of char is: %zu\n"              , sizeof(char));
  printf("size of short int is: %zu\n"         , sizeof(short int));
  printf("size of int is: %zu\n"               , sizeof(int));
  printf("size of long int is: %zu\n"          , sizeof(long int));

  printf("size of float is: %zu\n"             , sizeof(float));
  printf("size of double is: %zu\n"            , sizeof(double));

  printf("size of unsigned int is: %zu\n"      , sizeof(unsigned int));
  printf("size of unsigned long int is: %zu\n" , sizeof(unsigned long int));

  printf("size of pointer is: %zu\n" , sizeof(void*));

  printf("\n");

  // The size can also asked for from a variable
  { char a;
    printf("size of char is: %zu\n"              , sizeof(a));
  }
  { short int a;
    printf("size of short int is: %zu\n"         , sizeof(a));
  }
  { int a;
    printf("size of int is: %zu\n"               , sizeof(a));
  }
  { long int a;
    printf("size of long int is: %zu\n"          , sizeof(a));
  }

  { float a;
    printf("size of float is: %zu\n"             , sizeof(a));
  }
  { double a;
    printf("size of double is: %zu\n"            , sizeof(a));
  }

  { unsigned int a;
    printf("size of unsigned int is: %zu\n"      , sizeof(a));
  }
  { unsigned long int a;
    printf("size of unsigned long int is: %zu\n" , sizeof(a));
  }

  { int a;
    printf("size of pointer is: %zu\n" , sizeof(&a));
  }

  return 0;
}
