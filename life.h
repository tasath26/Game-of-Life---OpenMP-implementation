#include <stdio.h>  
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <omp.h>
#include <time.h>

struct timespec start_time,finish_time;

int init_grids(FILE *fp);

void game_of_life();
