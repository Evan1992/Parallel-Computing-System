#include "fft.h"
#include "utilities.h"
#include "public.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* ============ Input and Output============== */
char *input_file1;
char *input_file2;
char *output;
char *output_file = "../output/output_serial";

/* =========== Program Parameters ============ */
int N = MAXN;
complex *im1[N], *im2[N]; /* im1, im2 are two N*N images, where each element is a complex  */
complex *im3[N];

void main(int argc, char **argv){
    /* Process program parameters */
    if(argc==5){
        N = atoi(argv[1]);
        input_file1 = argv[2];
        input_file2 = argv[3];
        output = argv[4];
    }else{
        printf("Usage: %s <matrix_size> <input_file1> <input_file2> <output_file>\n", argv[0]);
        exit(0);
    }

    /* Initialize im1 and im2*/
    void initialize_inputs(){
        complex im1[N][N], im2[N][N];    /* im1, im2 are two N*N images, where each element is a complex  */
        complex im3[N][N];
    }

    /* Read data from the file and initialize im1 and im2*/
    arr=( int (*)[n])malloc(sizeof(int)*m*n);
    read_from_file(input_file1, im1);
    read_from_file(input_file2, im2);
    if(argc==4){
        load_data_from_file(given_output, given_im3);
    }

    /* ========== Computation =========== */
    /* record the start_time */
    double start_time = MPI_Wtime();

    /* 1D-FFT */
    int isign = -1;
    for(int row=0; row<N; row++){
        c_fft1d(im1[row], N, isign);
        c_fft1d(im2[row], N, isign);
    }

    /* transpose im1 and im2 */
    transpose(im1);
    transpose(im2);

    /* Another round of 1D-FFT */
    for(int row=0; row<N; row++){
        c_fft1d(im1[row], N, isign);
        c_fft1d(im2[row], N, isign);
    }

    /* tranpose im1 and im2 again */
    transpose(im1);
    transpose(im2);

    /* point-wise multiplication */
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

    /* transpose im3 */
    transpose(im3);

    /* Another round of 1D-FFT */
    for(int row=0; row<N; row++){
        c_fft1d(im3[row], N, isign);
    }

    /* transpose im3 again */
    transpose(im3);

    /* print elapsed time */
    double end_time = MPI_Wtime();
    printf("elapsed time of serial program is %1.4f seconds\n", end_time-start_time);

    // write the result to a file
    write_to_file(output_file, im3);

    // Compare output with given_output
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(im3[i][j].r != given_im3[i][j].r || im3[i][j].i != given_im3[i][j].i){
                printf("The answer is wrong!\n");
                exit(0);
            }
        }
    }
    printf("We get the right answer!\n");
}