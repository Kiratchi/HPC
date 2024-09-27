/*
 * - Null-terminated strings are described at
 *   https://en.cppreference.com/w/c/string/byte
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(
    int argc,
    char *argv[]
    )
{
  // Usual strings in C are encoded as ASCII, restricting them to the
  // American alphabeth. Each integer between 1 and 255 is assigned one
  // character.
  char a[] = "A string";
  for ( int ix = 0; ix < 9; ++ix )
    printf("character at position %i corresponds to integer %03i\n", ix, a[ix]);

  // Strings are null-terminated.
  printf("String before truncation: %s\n", a);
  a[5] = 0;
  printf("String after truncation: %s\n", a);

  // Conversion of strings to other types requires functions atoi, atof etc. from stdlib.h.
  printf("Converted integer %i\n", atoi("7"));
  printf("Converted float %f\n", atof("7.07"));


  char b[] = " is a string";
  char c[21];
  // You cannot write c = "A string"
  strcpy(c, "A string");
  strcat(c, b); // NOLINT
  printf("Concatenated strings are: %s\n", c);

  // When we know the size of strings and do not want to rely on
  // null-termination, we can use memset, memcpy, memmove. These are important
  // in general to copy content of one array to another memory.

  memcpy(c, "Another string", 8); // NOLINT
  // memmove is slower, but allows to overlap source and destination.
  memmove(c+7, c, 8); // NOLINT
  // Since we have only allocated 21 characters and one will be needed as
  // null-terminator, we have 20-15 characters maximum left. In particular,
  // "string" would not fit into the remaining positions.
  memcpy(c+15, "str", 3); // NOLINT
  c[18] = 0;
  printf("Handmade string is are: %s\n", c);

  return 0;
}
