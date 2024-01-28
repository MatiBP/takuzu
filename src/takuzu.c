#include "../include/takuzu.h"
#include "../include/grid.h"


/*
 * Function: grid_allocate
 * -----------------------
 * Allocates memory for a Takuzu grid of the specified size and initializes it with '_'.
 *
 * Parameters:
 *   - g: Pointer to the t_grid structure representing the Takuzu grid.
 *   - size: Size of the grid (number of rows/columns).
 *
 * Returns:
 *   - None
 *
 * Notes:
 *   - Exits the program with an error message if memory allocation fails.
 */
void grid_allocate(t_grid* g, int size) {
    // Allocate memory for the grid
    g->grid = (char*)calloc(size * size, sizeof(char));

    // Check if memory allocation was successful
    if (g->grid == NULL) {
        fprintf(stderr, "ERROR: Memory allocation for the grid failed. Exiting with error.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the grid with '_' (underscore) characters
    for (int i = 0; i < size * size; i++) {
        g->grid[i] = '_';
    }

    // Set the grid size
    g->size = size;
}


/*
 * Function: grid_free
 * -------------------
 * Frees the memory allocated for a Takuzu grid.
 *
 * Parameters:
 *   - g: Pointer to the t_grid structure representing the Takuzu grid.
 *
 * Returns:
 *   - None
 *
 * Notes:
 *   - Checks if the grid pointer is not NULL before freeing the memory.
 *   - Sets the grid pointer to NULL after freeing to avoid potential dangling pointers.
 */
void grid_free(t_grid* g) {
    if (g->grid != NULL) {
        free(g->grid);
        // Set the grid pointer to NULL to avoid potential dangling pointers
        g->grid = NULL;
    }
}


/*
 * Function: grid_print
 * --------------------
 * Prints the Takuzu grid to the specified file stream.
 *
 * Parameters:
 *   - g: Pointer to the t_grid structure representing the Takuzu grid.
 *   - fd: File stream where the grid will be printed (e.g., stdout, a file).
 *
 * Returns:
 *   - None
 *
 * Notes:
 *   - '#' characters are omitted from the printed output.
 *   - Rows are separated by newline characters.
 */
void grid_print(t_grid* g, FILE* fd) {
    for (int row = 0; row < g->size; row++) {
        for (int col = 0; col < g->size; col++) {
            // Print the grid element if it is not '#'
            if (g->grid[row * g->size + col] != '#') {
                fprintf(fd, "%c", g->grid[row * g->size + col]);
                // If not the last element in the row, print a space as a separator
                if (col < g->size - 1) {
                    fprintf(fd, " ");
                }
            }
        }
        // Print a newline character only if the line had content
        if (row < g->size) {
            fprintf(fd, "\n");
        }
    }
}


/*
 * Function: check_char
 * --------------------
 * Checks if a character is a valid Takuzu grid character ('0', '1', or '_').
 *
 * Parameters:
 *   - c: The character to be checked.
 *
 * Returns:
 *   - true if the character is valid, false otherwise.
 *
 * Notes:
 *   - Returns true if the character is '0', '1', or '_'; otherwise, returns false.
 */
bool check_char(const char c) {
    // Check if the character is a valid Takuzu grid character
    return (c == '0' || c == '1' || c == '_');
}


/*
 * Function: file_parser
 * ---------------------
 * Parses a Takuzu grid from a file and stores it in the given t_grid structure.
 *
 * Parameters:
 *   - grid: Pointer to the t_grid structure where the parsed grid will be stored.
 *   - filename: The name of the file containing the Takuzu grid.
 *
 * Returns:
 *   - EXIT_SUCCESS if the file is successfully parsed, EXIT_FAILURE otherwise.
 *
 * Notes:
 *   - The function reads the file character by character, ignoring comments marked with '#'.
 *   - It validates the grid size and characters and handles potential errors during parsing.
 *   - The parsed grid is stored in the provided t_grid structure.
 */
int file_parser(t_grid* grid, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "takuzu: error: Failed to open file\n");
        return EXIT_FAILURE;
    }

    char line[MAX_GRID_SIZE] = "";
    int gridSize = 0;
    int row = 0;
    bool started = false;
    char caractere_parsed;

    while (!started && (caractere_parsed = fgetc(file)) != EOF) {
        // Ignore comments in the file
        if (caractere_parsed == '#') {
            while (caractere_parsed != EOF && caractere_parsed != '\n') {
                caractere_parsed = fgetc(file);
            }
        }
        else if (caractere_parsed == '\n') {
            if (gridSize == 0) {
                continue;
            }
            if (gridSize != 4 && gridSize != 8 && gridSize != 16 && gridSize != 32 && gridSize != 64) {
                fprintf(stderr, "takuzu: error: line %d is malformed (wrong number of columns: %d)\n", row, gridSize);
                fclose(file);
                return EXIT_FAILURE;
            }
            else {
                started = true;
                grid_allocate(grid, gridSize);
            }
        }
        else if (check_char(caractere_parsed)) {
            line[gridSize] = caractere_parsed;
            gridSize++;
        }
        else if (caractere_parsed != ' ' && caractere_parsed != '\t') {
            fprintf(stderr, "takuzu: error: wrong character ‘%c’ at line %d!\n", caractere_parsed, row);
            fclose(file);
            grid_free(grid);
            return EXIT_FAILURE;
        }
    }
    if (caractere_parsed == EOF) {
        if (gridSize != 4 && gridSize != 8 && gridSize != 16 && gridSize != 32 && gridSize != 64) {
            fprintf(stderr, "takuzu: error: line %d is malformed (wrong number of columns: %d)\n", row, gridSize);
            fclose(file);
            return EXIT_FAILURE;
        }
        else {
            started = true;
            grid_allocate(grid, gridSize);
        }
    }
    // Copy the first line to the grid
    for (int col = 0; col < gridSize; col++) {
        grid->grid[col] = line[col];
    }

    row++;
    gridSize = 0;
    while ((caractere_parsed = fgetc(file)) != EOF) {
        if (caractere_parsed == '#') {
            while (caractere_parsed != EOF && caractere_parsed != '\n') {
                caractere_parsed = fgetc(file);
            }
        }
        else if (check_char(caractere_parsed)) {
            line[row * grid->size + gridSize] = caractere_parsed;
            gridSize++;
        }
        else if (caractere_parsed == '\n') {
            if (gridSize == 0) {
                continue;
            }
            if (grid->size != gridSize) {
                fprintf(stderr, "takuzu: error: line %d is malformed (wrong number of columns: %d)\n", row, gridSize);
                grid_free(grid);
                fclose(file);
                return EXIT_FAILURE;
            }
            else {
                for (int col = 0; col < grid->size; col++) {
                    grid->grid[row * grid->size + col] = line[row * grid->size + col];
                }
                gridSize = 0;
                row++;
            }
        }
        else if (caractere_parsed != ' ' && caractere_parsed != '\t') {
            fprintf(stderr, "takuzu: error: wrong character ‘%c’ at line %d!\n", caractere_parsed, row);
            grid_free(grid);
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    if (caractere_parsed == EOF && grid->size == gridSize) {
        if (row < grid->size) {
            // Copy the lines to the grid
            for (int col = 0; col < grid->size; col++) {
                grid->grid[row * grid->size + col] = line[row * grid->size + col];
            }
            row++;
        }
        else {
            row++;
        }
    }
    // Validate the number of rows in the file
    if (row != grid->size) {
        fprintf(stderr, "takuzu: error: Invalid number of rows in the file: row = %d et grid size = %d\n", row, grid->size);
        grid_free(grid);
        fclose(file);
        return EXIT_FAILURE;
    }

    fclose(file);
    return EXIT_SUCCESS;
}


/*
 * Function: print_usage
 * ---------------------
 * Print the usage information for the Takuzu program.
 */
void print_usage() {
    printf("\nUsage: takuzu [-a|-o FILE|-v|-h] FILE\n");
    printf("takuzu -g[N] [-u|-o FILE|-v|-N|-h]\n");
    printf("Solve or generate takuzu grids of size: 4, 8 16, 32, 64\n");
    printf("-a, --all search for all possible solutions\n");
    printf("-g[N], --generate[=N] generate a grid of size NxN (default: 8)\n");
    printf("-o FILE, --output FILE write output to FILE\n");
    printf("-u, --unique generate a grid with a unique solution\n");
    printf("-v, --verbose verbose output\n");
    printf("-n, --number%% to set the percentage of '0' and '1' characters in the grid (default: 50%%)\n");
    printf("-h, --help display this help and exit\n");
}


/*
 * Function: output_to_file
 * ------------------------
 * Write the given content to the specified file.
 *
 * Parameters:
 *   - filename: The name of the file to write to.
 *   - content: The content to write to the file.
 */
void output_to_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }
    fputs(content, file);
    fclose(file);
}


takuzu_Options option; //variable for options

/*
 * Function: initializeTakuzuOptions
 * ---------------------------------
 * Initialize the options structure with default values.
 *
 * Parameters:
 *   - options: Pointer to the takuzu_Options structure to initialize.
 */
void initializeTakuzuOptions(takuzu_Options* options) {
    options->verbose = false;
    options->unique = false;
    options->output_file = NULL;
    options->all = false;
    options->generate_mode = false;
    options->number = 50;
    options->grid_size = 8;
    options->mode = MODE_FIRST;
}


/*
 * Function: main
 * --------------
 *
 * Parameters:
 *   - argc: The number of command-line arguments.
 *   - argv: An array of strings representing the command-line arguments.
 *
 * Returns:
 *   - int: The exit status of the program.
 *
 * Notes:
 *   - Use -h for more informations
 */
int main(int argc, char* argv[]) {
    int c;
    int option_index = 0;

    initializeTakuzuOptions(&option);

    static struct option long_options[] = {
        {"all", no_argument, 0, 'a'},
        {"generate", optional_argument, 0, 'g'},
        {"unique", no_argument, 0, 'u'},
        {"verbose", no_argument, 0, 'v'},
        {"output", required_argument, 0, 'o'},
        {"number", optional_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, "ag::o:uvn::h", long_options, &option_index)) != -1) {
        switch (c) {
        case 'a':
            option.all = true;
            option.mode = MODE_ALL;
            break;
        case 'g':
            option.generate_mode = true;
            if (optarg != NULL) { //if no parameter with g the size is by default 8 
                int value = atoi(optarg);
                if (value != 4 && value != 8 && value != 16 && value != 32 && value != 64) {
                    fprintf(stderr, "Error: Invalid grid size specified for generation mode.\n");
                    print_usage();
                    exit(EXIT_FAILURE);
                }
                option.grid_size = value;
            }
            break;
        case 'o':
            if (optarg != NULL) { //if no parameter the output is stdout
                option.output_file = optarg;
            }
            break;
        case 'u':
            option.unique = true;
            if (option.unique) {
                fprintf(stderr, "Mode generate grid with unique solution activate.\n");
            }
            break;
        case 'v':
            option.verbose = true;
            if (option.verbose) {
                fprintf(stderr, "Mode verbose output activate.\n");
            }
            break;
        case 'n':
            if (optarg != NULL) { //if no parameter with N the % is by default 50%
                int number = atoi(optarg);
                if (number < 0 || number > 100) {
                    fprintf(stderr, "Error: Invalid N%% for the generation.\n");
                    print_usage();
                    exit(EXIT_FAILURE);
                }
                option.number = number;
            }
            break;
        case 'h':
            print_usage();
            exit(EXIT_SUCCESS);
        case '?':
            exit(EXIT_FAILURE);
        }
    }

    if (argc == 1) {
        fprintf(stderr, "Error: no input grid given!\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    if (option.unique && !option.generate_mode) {
        fprintf(stderr, "warning: option 'unique' conflict with solver mode, exiting!\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    if (option.all && option.generate_mode) {
        fprintf(stderr, "warning: option 'all' conflict with generate mode, exiting!\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    //We are un generate_mode
    if (option.generate_mode) {
        // Check if the grid size is specified
        if (option.grid_size <= 0) {
            fprintf(stderr, "Error: In generator mode, you need to specify a correct grid size.\n");
            print_usage();
            exit(EXIT_FAILURE);
        }
        t_grid generatedGrid;
        grid_allocate(&generatedGrid, option.grid_size);
        // Generate a random grid with the specified percentage 
        if (option.unique) {
            generate_random_grid_with_solution(&generatedGrid, option.number);  // You can adjust the percentage as needed with -n
        }
        else {
            generate_random_grid(&generatedGrid, option.number);
        }

        if (option.verbose && is_consistent(&generatedGrid)) {
            printf("\nWe generate a grid %d*%d with a generation of %d%%:\n\n", option.grid_size, option.grid_size, option.number);
            printf("\nDon't pay attention of verbose message, the grid is consistent\n");
        }
        if (option.output_file != NULL) {
            FILE* file = fopen(option.output_file, "w");
            if (file == NULL) {
                perror("Error when opening the file");
                exit(EXIT_FAILURE);
            }
            grid_print(&generatedGrid, file);
            fclose(file);
        }
        else {
            // Write to stdout
            grid_print(&generatedGrid, stdout);
        }
        grid_free(&generatedGrid);
    }

    // We are in solver mode, check if a grid file was provided as an argument
    if (optind < argc) {
        const char* filename = argv[optind];
        t_grid myGridPars;

        if (file_parser(&myGridPars, filename) == EXIT_SUCCESS) {
            if (is_consistent(&myGridPars)) {
                if (option.output_file != NULL) {
                    FILE* file = fopen(option.output_file, "w");
                    if (file == NULL) {
                        perror("Error when opening the file");
                        exit(EXIT_FAILURE);
                    }
                    if (is_valid(&myGridPars)) {
                        printf("The grid is already valid.\n");
                        grid_print(&myGridPars, file);
                        fclose(file);
                        grid_free(&myGridPars);
                        exit(EXIT_SUCCESS);
                    }
                    else {
                        grid_solver(&myGridPars, option.mode);
                        grid_print(&myGridPars, file);
                        fclose(file);
                    }
                }
                else {
                    if (is_valid(&myGridPars)) {
                        printf("The grid is already valid.\n");
                        grid_print(&myGridPars, stdout);
                        grid_free(&myGridPars);
                        exit(EXIT_SUCCESS);
                    }
                    else {
                        grid_solver(&myGridPars, option.mode);

                        grid_print(&myGridPars, stdout);// Write to stdout
                        if (option.verbose && is_consistent(&myGridPars)) {
                            printf("You activate Verbose, don't panic the grid is consistent\n");
                        }
                    }
                }
            }
            else {
                fprintf(stderr, "The grid is not consistent.\n");
                grid_free(&myGridPars);
                exit(EXIT_FAILURE);
            }
            grid_free(&myGridPars);
        }
        else {
            fprintf(stderr, "\nFailed to parse grid from file '%s'\n", filename);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}