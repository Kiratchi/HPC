#include <stdio.h>
#include "potential_header.h"

int
main(
    int argc,
    char *argv[]
)
{
    printf("command line argumetns:\n");

    for ( int ix = 0; ix < argc; ++ix) {
        printf("%s\n", argv[ix]);
    }

    return 0;
}