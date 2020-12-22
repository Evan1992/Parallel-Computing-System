#include "public.h"
#include <stdio.h>

void read_from_file(char* file_name, complex (*im)[N]){
    FILE *f; /*open file descriptor */
    f = fopen(file_name, "r");
    int i, j;
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            fscanf(f,"%g",&im[i][j].r);
    fclose(f);
}

void load_data_from_file(char* file_name, complex (*im)[N]){
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
    for(i=0; i<N; i++)
        for(j=0; j<N; j++)
            fprintf(f, "%16.7e", im[i][j].r);
        fprintf(f, "\n");
}

void transpose(complex (*im)[N]){
    int i, j;
    for(i=0; i<N; i++){
        for(j=i+1; j<N; j++){
            complex tmp = im[i][j];
            im[i][j] = im[j][i];
            im[j][i] = tmp;
        }
    }
}

complex multiplication(complex c1, complex c2){
    complex c3;
    c3.r = c1.r*c2.r - c1.i*c2.i;
    c3.i = c1.r*c2.i + c2.r*c1.i;
    return c3;
}