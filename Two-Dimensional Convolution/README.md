# Introduction
This is a program to compute 2D convolution parallelly

# Instructions
How to compile the programs and run the executables to solve 2D convolution problem

========== Import input file and set the parameters ============
1. Put all input files in the **input** folder
2. Find **public.h** file and modify **N(matrix size)** manually, **N(matrix size)** is 512 by default.

=================== Compile the source code =====================
1. Open the terminal of the Ubuntu operating system, go to current directory
2. Simply use *make* to compile all the source code.
3. After complilation, you will get 3 executables in current path for you to do the calculation

===================== Run the executables =======================
## Usage
* To run the serial program
    ./serial <input_file1> <input_file2> <output_file>
* To run the parallel program
    mpirun -np <num_process> ./executable <input_file1> <input_file2> <output_file>
## Example
mpirun -np 4 ./mpi_data_parallelism "1_im1" "1_im2" "mpi_data_parallelism"