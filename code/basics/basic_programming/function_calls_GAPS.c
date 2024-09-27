#include <stdio.h>

void printf_from_snd_argument(/*INSERT*/);

int
main(
    int argc,
    char *argv[]
    )
{
  printf("command line arguments\n");

  printf_from_snd_argument(/*INSERT*/, argv);

  return 0;
}

/*INSERT*/
printf_from_snd_argument(
    int argc,
    char */*INSERT*/[]
    )
{
  for ( int ix = 1; ix < argc; ++ix ) {
    printf("%s\n", argv[ix]);
  }
}
