/*
 * Serial program to solve 2D convolution problem
 */

#include "fft.h"
#include "utilities.h"
#include "public.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============ Input and Output============== */
char input_file1[30] = "./input/";
char input_file2[30] = "./input/";
char output_file[30] = "./output/";

/* =========== Program Parameters ============ */
complex im1[N][N], im2[N][N]; /* im1, im2 are two N*N images, where each element is a complex  */
complex im3[N][N]; /* im3 is the result image */

void main(int argc, char **argv){
    /* Process program parameters */
    if(argc==4){
        strcat(input_file1, argv[1]);
        strcat(input_file2, argv[2]);
        strcat(output_file, argv[3]);
        printf("input_file1 is %s, input_file2 is %s, output_file is %s\n", input_file1, input_file2, output_file);
    }else{
        printf("Usage: %s <input_file1> <input_file2> <output_file>\n", argv[0]);
        exit(0);
    }

    /* Read data from the file and initialize im1 and im2 */
    read_from_file(input_file1, im1);
    read_from_file(input_file2, im2);
    printf("read data from file successfully\n");
    
    /* ========== Computation =========== */
    /* 1D-FFT */
    int isign = -1;
    for(int row=0; row<N; row++){
        c_fft1d(im1[row], N, isign);
        c_fft1d(im2[row], N, isign);
    }

    /* Transpose im1 and im2 */
    transpose(im1);
    transpose(im2);

    /* Another round of 1D-FFT */
    for(int row=0; row<N; row++){
        c_fft1d(im1[row], N, isign);
        c_fft1d(im2[row], N, isign);
    }

    /* Tranpose im1 and im2 again */
    transpose(im1);
    transpose(im2);

    /* Point-wise multiplication */
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            im3[i][j] = multiplication(im1[i][j], im2[i][j]);
        }
    }

    /* im3 inverse 1D-FFT */
    isign = 1;
    for(int row=0; row<N; row++){
        c_fft1d(im3[row], N, isign);
    }

    /* Transpose im3 */
    transpose(im3);

    /* Another round of 1D-FFT */
    for(int row=0; row<N; row++){
        c_fft1d(im3[row], N, isign);
    }

    /* transpose im3 again */
    transpose(im3);

    /* Write the result to a file */
    write_to_file(output_file, im3);
}