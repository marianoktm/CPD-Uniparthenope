#ifndef PROGETTOCALCOLO_P21FUN_H
#define PROGETTOCALCOLO_P21FUN_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <math.h>


// MAX_RAND for mtxtxt
#define MAX_RAND 256


// Matrix record
struct matrix {
    int ** mtx_ptr;
    int M;
};


// Returns no of digits of an int
int int_digits_no(int num);

// Returns a random number between 1 and max_rand
int rand_no(int max_rand);

// Prints error_str on stderr and then terminate the process with exit_code.
void easy_stderr(const char * error_str, int exit_code);

// If main_argc (actual argc) < exp_argc (expected argc), then terminate the process.
void argcheck(int main_argc, int exp_argc);

// Returns matrix file descriptor.
int matrix_init(const char * matrix_path);

// Opens a matrix file with O_RDWR | O_CREAT | O_APPEND | O_TRUNC
int matrix_rdwr_append_init(const char * matrix_path);

// Creates a MxM matrix on the file opened on matrix fd.
void file_matrix_build(int matrix, int M);

// Return 1D char array containing the whole matrix file.
char * matrix_read(int matrix_fd);

// Returns M if the matrix is square, 0 if it's not.
int matrix_check_square(char * file_buf, const char * delimit, const char * delimit2);

// Returns an empty 2D int array (dinamically allocated).
int ** alloc_2D_array(const int size);

// Builds the actual square matrix in mtx_ptr from file_buf
void matrix_build(int ** mtx_ptr, char * file_buf, const char * delimit, const char * delimit2);

// Builds the matrix as an array of size MxM
void matrix_build_as_array(int * mtx_array_ptr, char * file_buf, const char * delimit, const char * delimit2);

// Prints a row x col matrix
void matrix_print(int ** mtx, int row, int col);

// Prints an MxM matrix
void matrix_square_print(int ** mtx, int m);

// Returns a 2D int array containing the matrix.
struct matrix * matrix_alloc(int main_matrix_fd);

// Returns the offset for block extracting
struct offset offset_gen();

// Returns the (M/np)x(M/np) block
struct matrix * matrix_block_extract(struct matrix * mtx, int np, int np_no);

// Returns an npxnp matrix with all elements initialized to 0
struct matrix * initialize_sum_matrix(int size);

// Returns the sum of mat1 and mat2
struct matrix * matrix_sum(struct matrix * mat1, struct matrix * mat2);

// Returns -1 if the matrices are not the same, 0 otherwise
int matrix_compare(struct matrix  * mat1, struct matrix * mat2);

#endif //PROGETTOCALCOLO_P21FUN_H
