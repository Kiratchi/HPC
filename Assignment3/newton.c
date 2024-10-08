#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <threads.h>

// Some space 
// For all
// Beautiful Functioninitializations
static inline void determine_command_line(int argc, char const *argv[]);

// GLOBAL VARIABLES
int T;
int L;
int power;

int main(int argc, char const *argv[])
{
    /* code - WRite the code in the format of functions, as much as possible <3 */
    
    determine_command_line(argc, argv);
    printf("T: %d, L: %d, power: %d\n", T, L, power);   // Ha kvar denna sålänge!

 
    int x = 0;
    int x_new = x - (pow(x,power) - 1) / (power * pow(x,power-1));
    while (abs(x_new - x) > 0.001){
        x = x_new;
        x_new = x - (pow(x,power) - 1) / (power * pow(x,power-1));
    }



    return 0;
}

// Some space 
// For all
// Beautiful Functions
static inline void determine_command_line(int argc, char const *argv[]){
    if (argc != 4){
        fprintf(stderr, "Usage: %s -tT -lL X where T is threads, L is pixels, X is power\n", argv[0]);
        exit(1);
    }
    for (int i = 1; i < argc-1; i++) {
        if (argv[i][0] == '-') {
            int value = atoi(&argv[i][2]);  
            if (argv[i][1] == 't') {
                T = value;
            } else if (argv[i][1] == 'l') {
                L = value;
            } else {
                fprintf(stderr, "Unknown option: %s\n, should be -t, -l", argv[i]);
                exit(1);
            }
        } else {
            fprintf(stderr, "Unknown option: %s\n, should start with -", argv[i]);
            exit(1);
        } 
    }
    power = atoi(argv[3]);
}