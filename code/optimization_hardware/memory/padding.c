#include <stdio.h>

typedef struct {
  double d1, d2, d3, d4, d5;
  char   c1, c2, c3, c4, c5;
} data_nopadding;

typedef struct {
  double d1; char c1;
  double d2; char c2;
  double d3; char c3;
  double d4; char c4;
  double d5; char c5;
} data_padding;

int
main(
  int argc,
  char *argv[]
  )
{  
  // Specific data types have to or should start at specific positions in
  // memory.  For example, the address of variable of type double should be
  // divisible by 4 or 8. To achieve this, the compiler adds padding to
  // structures.
 
  printf("size without padding: %zu\n", sizeof(data_nopadding));
  printf("size with padding: %zu\n", sizeof(data_padding));

  return 0;
}
