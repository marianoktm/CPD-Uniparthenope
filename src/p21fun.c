#include "p21fun.h"

// Ritorna il numero di cifre di un intero
int int_digits_no(int num) {
    return (int) floor(log10(abs(num))) + 1;
}

// Ritorna un int randomico compreso tra -max_rand e +max_rand
int rand_no(int max_rand) {
    if (rand() % 2 == 0) {
        return rand() % max_rand;
    }
    else {
        return -(rand() % max_rand);
    }
}

// Quando richiamata, stampa la stringa error_str sullo stderr e termina il programma con codice di uscita exit_code
void easy_stderr(const char * error_str, const int exit_code) {
    fprintf(stderr, "%s\n", error_str);
    fflush(stderr);
    exit(exit_code);
}

// Controlla se il numero di argomenti del main è corretto
void argcheck(const int main_argc, const int exp_argc) {
    // main_argc è il numero di argomenti del main. exp_argc è il numero di argomenti che ci aspettiamo. se main_argc è inferiore, errore.
    if (main_argc < exp_argc)
        easy_stderr("invalid arguments number!", -1);
}

// Apre il file nel path matrix_path e ritorna il descrittore sul quale è stato aperto il file, o esce dal programma se il file non esiste
int matrix_init(const char * matrix_path) {
    int fd;

    // Se il file non può essere aperto, errore.
    if ((fd = open(matrix_path, O_RDONLY, 0666)) < 1 )
        easy_stderr("matrix's file can't be opened or does not exists!", -1);

    // *** DEBUG *** printf("matrix on fd %d\n", fd);

    return fd;
}

// Come matrix_init ma apre il file in read and write, lo crea se non esiste, qualsiasi scrittura va alla fine del file e se esiste quando lo apre ne cancella il contenuto.
int matrix_rdwr_append_init(const char * matrix_path) {
    int fd;
    // Se il file non può essere aperto, errore.
    if ((fd = open(matrix_path, O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666)) < 1 )
        easy_stderr("matrix's file can't be opened or created!", -1);
    // *** DEBUG *** printf("matrix on fd %d\n", fd);
    return fd;
}

// Scrive nel file puntato dal descrittore matrix una matrice randomica MxM
void file_matrix_build(int matrix, int M) {
    // Si imposta il rand seed
    srand(time(0) + 1 );
    // Buffer per il numero casuale generato. Verrà poi scritto in matrix
    char * buf = (char *) malloc(int_digits_no(MAX_RAND));
    // Separatori
    char * endline = "\n";
    char * space = " ";
    // Scrittura nel file
    for (int row = 0; row < M; row++) {
        for (int col = 0; col < M; col++) {
            // Scrittura di un numero casuale (in stringa) su buf
            sprintf(buf, "%d", rand_no(MAX_RAND));
            printf("buf %d: %s\n", col, buf);
            // Scrittura del numero + uno spazio
            write(matrix, buf, strlen(buf));
            write(matrix, space, strlen(space));
        }
        // Scrittura dell'accapo
        write(matrix, endline, strlen(endline));
    }
}

// Copia il file aperto su matrix_fd in un buffer (array) di char
char * matrix_read(const int matrix_fd) {
    // Controlla quanto è grande il file ed alloca abbastanza memoria per contenerlo in un buffer di char
    off_t tot_bytes = lseek(matrix_fd, 0, SEEK_END);
    char * file_buf_to_return = (char *) malloc(sizeof(char) * tot_bytes);
    // Reset del file offset (per sicurezza)
    lseek(matrix_fd, 0, SEEK_SET);
    // Legge tutta la matrice dal file in un buffer
    int read_bytes = 0;
    if ((read_bytes = read(matrix_fd, file_buf_to_return, tot_bytes)) <= 0)
        return NULL;
    // *** DEBUG *** printf("tot_bytes %lld, read_bytes %d\n\n", tot_bytes, read_bytes);
    return file_buf_to_return;
}

// Controlla se la matrice è quadrata
int matrix_check_square(char * file_buf, const char * delimit, const char * delimit2) {
    // Variabili necessarie al funzionamento di strtok_r
    char * tok, * tok2;
    tok = tok2 = NULL;
    char * last, * last2;
    last = last2 = NULL;
    // Variabili per contare il numero di righe e colonne
    int row_count = 0;
    int col_count = 0;
    int last_col_count = 0;
    // Il for esterno conta le righe, il for interno conta le colonne
    for (tok = strtok_r(file_buf, delimit, &last); tok; tok = strtok_r(NULL, delimit, &last)) {
        col_count = 0; // Conta gli elementi sulla riga per ogni iterazione
        row_count++;
        for (tok2 = strtok_r(tok, delimit2, &last2); tok2; tok2 = strtok_r(NULL, delimit2, &last2))
            col_count++;
        // non serve confrontare col_count e last_col_count sulla prima iterazione
        // If col_count != last_col_count la matrice non è quadrata
        if (row_count > 1 && col_count != last_col_count)
            return 0;
        // *** DEBUG *** printf("row: %d, col %d, last %d\n\n", row_count, col_count, last_col_count);
        last_col_count = col_count;
    }
    // Si controlla se la matrice non è quadrata
    if (col_count != row_count) {
        return 0;
    }
    // Si ritorna il numero di colonne (andrebbe bene anche il numero di righe perchè la matrice è quadrata)
    return last_col_count;
}

// Alloca dinamicamente una matrice
int ** alloc_2D_array(const int size) {
    int ** matrix_out_ptr = (int **) malloc(sizeof(int *) * size);
    for (int i = 0; i < size; ++i)
        matrix_out_ptr[i] = (int *) malloc(sizeof (int) * size);
    return matrix_out_ptr;
}

// Trasforma un array di char in una matrice di int
void matrix_build(int ** mtx_ptr, char * file_buf, const char * delimit, const char * delimit2) {
    // Vars for strtok_r
    char * tok, * tok2;
    tok = tok2 = NULL;
    char * last, * last2;
    last = last2 = NULL;
    int row_index, col_index;
    row_index = col_index = 0;
    for (tok = strtok_r(file_buf, delimit, &last); tok; tok = strtok_r(NULL, delimit, &last)) {
        col_index = 0;
        for (tok2 = strtok_r(tok, delimit2, &last2); tok2; tok2 = strtok_r(NULL, delimit2, &last2)) {
            mtx_ptr[row_index][col_index] = (int) strtol(tok2, NULL, 10);
            col_index++;
        }
        row_index++;
    }
}

// Stampa una matrice rowxcol
void matrix_print(int ** mtx, int row, int col) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("[%d] ", mtx[i][j]);
        }
        printf("\n");
    }
}

// Wrapper per matrix_print che stampa una matrice MxM
void matrix_square_print(int ** mtx, int m) {
    matrix_print(mtx, m, m);
}

// Ritorna una matrice costruita partendo dal file aperto sul descrittore main_matrix_fd
struct matrix * matrix_alloc(const int main_matrix_fd) {
    // Si legge la matrice in un buffer (array di char)
    char * file_buf;
    if ((file_buf = matrix_read(main_matrix_fd)) == NULL)
        easy_stderr("matrix was not read on file_buf!", -1);
    // *** DEBUG *** printf("%s\n", file_buf);
    // Si controlla se la matrice è quadrata
    char delimit[] = "\n";
    char delimit2[] = " ";
    int M; // Sarà poi il size della matrice
    // Si passa una copia del buffer perchè strtok_r modifica la stringa sulla quale lavora
    char * buf_to_pass = (char *) malloc(sizeof(char) * strlen(file_buf));
    if ((M = matrix_check_square(strcpy(buf_to_pass, file_buf), delimit, delimit2)) == 0) {
        free(file_buf);
        free(buf_to_pass);
        easy_stderr("the matrix is not square!", -1);
    }
    // Si alloca dinamicamente una matrice di interi di dimensione MxM
    int ** matrix_ptr = alloc_2D_array(M);
    // Si assegnano i valori alla matrice
    matrix_build(matrix_ptr, strcpy(buf_to_pass, file_buf), delimit, delimit2);
    // *** DEBUG *** matrix_print(matrix_ptr, M, M);
    // Si ritorna una struttura Matrix (come se fosse un oggetto matrix)
    struct matrix * to_return = (struct matrix *) malloc(sizeof(struct matrix));
    to_return->mtx_ptr = matrix_ptr;
    to_return->M = M;
    return to_return;
}

// Estrae un blocco dalla matrice mtx in base al numero di processi (npxnp) ed al thread (np_id) che deve estrarre il blocco
struct matrix * matrix_block_extract(struct matrix * mtx, int np, int np_id) {
    // Si calcola la dimensione del blocco
    int block_size = mtx->M / np;
    // Si assegnano degli alias (per semplificare la lettura del codice) e si calcolano gli offset
    int block_no = np*np;
    // Si calcola il resto della divisione tra np_id*block_size e M
    int col_offset = (np_id * block_size) % mtx->M;
    // Si moltiplica il risultato intero della divisione np_id/np per la dimensione del blocco
    int row_offset = np_id / np * block_size;
    // *** DEBUG *** printf("[Thread %d]:\nBlock size: %d\nCol offset: %d\nRow offset: %d\n\n", np_id, block_size, col_offset, row_offset);
    // Allocazione del blocco
    struct matrix * out = (struct matrix *) malloc(sizeof(struct matrix));
    out->mtx_ptr = alloc_2D_array(block_size);
    out->M = block_size;
    // Assegnazione del blocco
    for (int row = 0; row < block_size ; row++) {
        for (int col = 0; col < block_size; col++) {
            out->mtx_ptr[row][col] = mtx->mtx_ptr[row + row_offset][col + col_offset];
        }
    }
    return out;
}

// Ritorna una matrice di dimensione size inizializzata con tutti gli elementi a 0
struct matrix * initialize_sum_matrix(int size) {
    // Allocating the matrix
    struct matrix * out = (struct matrix *) malloc(sizeof(struct matrix));
    out->mtx_ptr = alloc_2D_array(size);
    out->M = size;
    // Initializing all matrix's elements to 0
    for (int i = 0; i < out->M; ++i) {
        for (int j = 0; j < out->M; ++j) {
            out->mtx_ptr[i][j] = 0;
        }
    }
    return out;
}

// Effettua la somma tra due matrici quadrate mat1 e mat2.
struct matrix * matrix_sum(struct matrix * mat1, struct matrix * mat2) {
    // *** DEBUG *** printf("[Thread %d]: mat1 size: %d, mat2 size: %d\n", omp_get_thread_num(), mat1->M, mat2->M);
    // La somma si può effettuare solo se le matrici hanno size uguale
    if (mat1->M != mat2->M)
        easy_stderr("You can't sum up matrices with different size!", -1);
    // Si alloca la matrice di output
    struct matrix * out = (struct matrix *) malloc(sizeof(struct matrix));
    out->mtx_ptr = alloc_2D_array(mat1->M);
    out->M = mat1->M;
    //Somma effettiva
    for (int i = 0; i < out->M; ++i) {
        for (int j = 0; j < out->M; ++j) {
            out->mtx_ptr[i][j] = mat1->mtx_ptr[i][j] + mat2->mtx_ptr[i][j];
        }
    }
    return out;
}

// Funzione di debug che compara due matrici
int matrix_compare(struct matrix * mat1, struct matrix * mat2) {
    if (mat1->M != mat2->M) {
        return -1;
    }
    for (int i = 0; i < mat1->M; ++i) {
        for (int j = 0; j < mat1->M; ++j) {
            if (mat1->mtx_ptr[i][j] != mat2->mtx_ptr[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}
