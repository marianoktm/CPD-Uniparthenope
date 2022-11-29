#include "p21fun.h"


/* Main arguments:
 * argv[1]: file containing the matrix
 * argv[2]: M
 * */
int main(int argc, char * argv[]) {
    argcheck(argc, 3);

    // Opening matrix file
    int matrix_file = matrix_rdwr_append_init(argv[1]);

    // Building the matrix
    file_matrix_build(matrix_file, (int) strtol((argv[2]), NULL, 10));

    exit(0);
}
