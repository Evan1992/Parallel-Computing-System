#include "public.h"
#include <stdio.h>
#include <stdlib.h>

void read_from_file(char* file_name, complex (*im)[N]){
    FILE *f; /*open file descriptor */
    f = fopen(file_name, "r");
    int i, j;
    for (i=0;i<512;i++)
        for (j=0;j<512;j++)
            fscanf(f,"%g",&im[i][j].r);
    fclose(f);
}

void write_to_file(char *file_name, complex (*im)[N]){
    FILE *f;
    f = fopen(file_name, "w");
    int i, j;
    for(i=0; i<512; i++)
        for(j=0; j<512; j++)
            fprintf(f, "%16.7e", im[i][j].r);
        fprintf(f, "\n");
}

void transpose(complex (*im)[N]){

}