/*
 * Implement 2D convolution using MPI collective communication functions
 *  - MPI_Scatter
 *  - MPI_Gather
 */

#include "fft.h"
#include "utilities.h"
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

/* ========== Parameters =========== */
int size, rank;
MPI_Datatype mpi_complex;

/* ============ Input and Output============== */
char input_file1[30] = "./input/";
char input_file2[30] = "./input/";
char output_file[30] = "./output/";

/* =========== Program Parameters ============ */
complex im1[N][N], im2[N][N]; /* im1, im2 are two N*N images, where each element is a complex  */
complex im3[N][N]; /* im3 is the result image */
complex im_recv1[N][N], im_recv2[N][N], im_recv3[N][N];

/* ========== Prototype ============ */
void initialize_mpi_complex();
void mpi_fft(int isign, complex (*im)[N]);
void mpi_multiplication();

/* ========= Main Program ========== */
void main(int argc, char **argv){
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank==0){
        /* Process program parameters */
        if(argc == 4){
            strcat(input_file1, argv[1]);
            strcat(input_file2, argv[2]);
            strcat(output_file, argv[3]);
            printf("input_file1 is %s, input_file2 is %s, output_file is %s\n", input_file1, input_file2, output_file);
        }else{
            printf("Usage: mpirun -np <num_process> %s <input_file1> <input_file2> <output_file>\n", argv[0]);
            exit(0);
        }
        /* Read data from the file and initialize im1 and im2 */
        read_from_file(input_file1, im1);
        read_from_file(input_file2, im2);
        printf("read data from file successfully\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);

    /* Create a new mpi datatype */
    initialize_mpi_complex();

    /* ========== Computation =========== */
    /* Record the start_time */
    double start_time = MPI_Wtime();

    /* 1D-FFT */
    int isign = -1;
    mpi_fft(isign, im1);
    mpi_fft(isign, im2);

    /* Transpose im1 and im2 */
    if(rank==0){
        transpose(im1);
        transpose(im2);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    /* Another round of 1D-FFT */
    mpi_fft(isign, im1);
    mpi_fft(isign, im2);

    /* Tranpose im1 and im2 again */
    if(rank==0){
        transpose(im1);
        transpose(im2);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    /* Do the point-wise multiplication parallelly */
    mpi_multiplication();

    /* im3 inverse 1D-FFT */
    isign = 1;
    mpi_fft(isign, im3);

    /* Transpose im3 */
    if(rank==0){
        transpose(im3);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    /* Another round of 1D-FFT */
    mpi_fft(isign, im3);

    /* Transpose im3 again */
    if(rank==0){
        transpose(im3);
    }

    /* Print elapsed time of each process*/
    double end_time = MPI_Wtime();
    printf("elapsed time of process %d for computation is %1.4f seconds\n", rank, end_time-start_time);

    /* Write the result to a file */
    if(rank==0){
        write_to_file(output_file, im3);
    }

    MPI_Type_free(&mpi_complex); 
    MPI_Finalize();
}

void initialize_mpi_complex(){
    MPI_Type_contiguous(2, MPI_FLOAT, &mpi_complex);
    MPI_Type_commit(&mpi_complex);
}

void mpi_fft(int isign, complex (*im)[N]){
    // Send portion of rows from process 0 to other process using MPI_Scatter
    MPI_Scatter(im, N*N/size, mpi_complex, im_recv1, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
    // 1D-fft computation
    int row;
    for(row=0; row<N/size; row++){
        c_fft1d(im_recv1[row], N, isign);
    }
    MPI_Gather(im_recv1, N*N/size, mpi_complex, im, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
}

void mpi_multiplication(){
    // Send portion of rows from process 0 to other process using MPI_Scatter
    MPI_Scatter(im1, N*N/size, mpi_complex, im_recv1, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
    MPI_Scatter(im2, N*N/size, mpi_complex, im_recv2, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
    MPI_Scatter(im3, N*N/size, mpi_complex, im_recv3, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    // point-wise multiplication
    int i, j;
    for(i=rank*size; i<N/size; i++){
        for(j=0; j<N; j++){
            im_recv3[i][j] = multiplication(im_recv1[i][j], im_recv2[i][j]);
        }
    }
    MPI_Gather(im_recv1, N*N/size, mpi_complex, im1, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
    MPI_Gather(im_recv2, N*N/size, mpi_complex, im2, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
    MPI_Gather(im_recv3, N*N/size, mpi_complex, im3, N*N/size, mpi_complex, 0, MPI_COMM_WORLD);
}