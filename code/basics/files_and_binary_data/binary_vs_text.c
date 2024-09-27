/*
 * - All file related functions are described at
 *   https://en.cppreference.com/w/c/io
 */
 
#include <stdio.h>

int
main(
    int argc,
    char *argv[]
    )
{
  int a[] = {1, 1, 2, 3, 5, 8, 13, 21};

  // We can name files freely, an in particlar no extensions are needed. But it
  // is a good habbit to follow a consistent pattern.
  FILE * file = fopen("test_file.txt", "w");
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }

  for ( int ix = 0; ix < 7; ++ix )
    fprintf(file, "%i ", a[ix]);
  fprintf(file, "%i\n", a[7]);

  printf("The file has size %li\n", ftell(file));
  fclose(file);


  // Handling text files is much slower, since data must be converted to text
  // and vice versa.
  file = fopen("test_file.txt", "r");
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }

  for ( int ix = 0; ix < 8; ++ix ) {
    int b;
    int r = fscanf(file, "%i", &b);
    if ( r != 1 )
      printf("error reading from file\n");
    printf("read %i ", b);
  }
  { int b;
    printf("\nend of file return value is %i\n", fscanf(file, "%i", &b));
  }

  fclose(file);


  file = fopen("test_file.txt", "a");
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }
  // Buffering for text files can, in some situations, be best linewise. For
  // bianry data, if you tweak you write size, it can make sense to disable
  // buffering.
  // _IOFBF full buffering
  // _IOLBF line buffering
  // _IONBF no buffering
  setvbuf(file, NULL, _IOLBF, BUFSIZ);

  for ( int ix = 0; ix < 10; ++ix )
    fprintf(file, "line %i\n", ix);

  fclose(file);

  return 0;
}
