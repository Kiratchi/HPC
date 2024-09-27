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
  // To read or write files, we need to open and close them. By analogy, one
  // can think of this as a variant of allocating memory and freeing it.
  
  int a[] = {1, 1, 2, 3, 5, 8, 13, 21};

  // When opening a file, we specify whether we want to
  // r read from it,
  // w write to it (overwrite it if it exists),
  // a append to it (create it if it does not exists).
  FILE *file =  fopen("test_file.dat", "w");

  // As with malloc, we have to check the return value.
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }
    


  // When writing we pass the size of one array element and the number of
  // elements that we want to write.
  fwrite((void*) a, sizeof(int), 8, file);
  fclose(file);


  int b[12];

  file =  fopen("test_file.dat", "r");
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }
  // As with writing we pass the size of one array element and the number of
  // elements that we want to write.
  fread((void*) b, sizeof(int), 8, file);
  fclose(file);

  printf("read after first write: ");
  for ( int ix = 0; ix < 7; ++ix )
    printf("%i ", b[ix]);
  printf("%i\n", b[7]);


  int c[] = {34, 55, 89, 144};

  // When appending to a file, the file position indicator is placed at the end
  // of the file.
  file =  fopen("test_file.dat", "a");
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }
  fwrite((void*) c, sizeof(int), 4, file);
  fclose(file);


  file =  fopen("test_file.dat", "r");
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }
  fread((void*) b, sizeof(int), 12, file);
  fclose(file);

  printf("read after second write: ");
  for ( int ix = 0; ix < 11; ++ix )
    printf("%i ", b[ix]);
  printf("%i\n", b[11]);


  for ( int ix = 0; ix < 8; ++ix )
    a[ix] = ix;

  // To open for both reading and writing possibilities are:
  // r+ does not overwrite existing file
  // w+ overwrites existing file
  // a+ appends to existing file
  file =  fopen("test_file.dat", "r+");
  if ( file == NULL ) {
    printf("error opening file\n");
    return -1;
  }
  fwrite(a, sizeof(int), 8, file);

  // Writing to files can be buffered, so we have to flush the previous write
  // before reading.
  fflush(file);

  // When setting the file position indicator the second argument is an offset
  // from the origin, passed as the third argument. Possible values for the orgin are:
  // SEEK_SET the beginning of the file
  // SEKK_CUR the current file position indicator
  // SEKK_END the end of the file
  printf("The file position indicator is at %li\n", ftell(file));
  fseek(file, 0, SEEK_SET);
  // Alternatively, we could have used
  // fseek(file, -32, SEEK_CUR);
  printf("The file position indicator is at %li\n", ftell(file));
  fread(b, sizeof(int), 12, file);
  printf("The file position indicator is at %li\n", ftell(file));
  

  printf("read after third write: ");
  for ( int ix = 0; ix < 11; ++ix )
    printf("%i ", b[ix]);
  printf("%i\n", b[11]);


  fseek(file, 0, SEEK_END);
  printf("The file has size %li\n", ftell(file));


  // You cannot open arbitrarily many files: There is a limitied number of
  // "file descriptors". Closing files, gives back file descriptors.
  fclose(file);


  return 0;
}
