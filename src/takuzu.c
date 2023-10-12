#include "../include/takuzu.h"

void print_usage() {
    printf("Usage: takuzu [-a|-o FILE|-v|-h] FILE...\n");
    printf("takuzu -g[N] [-u|-o FILE|-v|-h]\n");
    printf("Solve or generate takuzu grids of size: 4, 8 16, 32, 64\n");
    printf("-a, --all search for all possible solutions\n");
    printf("-g[N], --generate[=N] generate a grid of size NxN (default: 8)\n");
    printf("-o FILE, --output FILE write output to FILE\n");
    printf("-u, --unique generate a grid with a unique solution\n");
    printf("-v, --verbose verbose output\n");
    printf("-h, --help display this help and exit\n");
}

void output_to_file(const char* filename, const char* content) {
    FILE *file = fopen(filename, "w"); // open in write mode
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }
    fputs(content, file);
    fclose(file);
}


int main(int argc, char*argv[]) {
    int c;
    int option_index = 0;
    bool verbose = false;
    bool unique = false;
    char *output_file = NULL;
    bool all = false;
    bool generate_mode = false;
    int grid_size = 8;
 
    static struct option long_options[] = {
        {"all", no_argument, 0, 'a'},
        {"generate", optional_argument, 0, 'g'},
        {"unique", no_argument, 0, 'u'},
        {"verbose", no_argument, 0, 'v'},
        {"output", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, "ag::o:uvh", long_options, &option_index)) != -1) {
        switch(c) {
            case 'a':
                all = true;
                break;
            case 'g':
                generate_mode = true;
                if(optarg != NULL){
                int value = atoi(optarg);
                if (value != 4 && value != 8 && value != 16 && value != 32 && value != 64) {
                    fprintf(stderr, "Error: Invalid grid size specified for generation mode.\n");
                    print_usage();
                    exit(EXIT_FAILURE);
                }
                grid_size = value;
                }
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'u':
                unique = true;
                if (unique) {
                    fprintf(stdout, "Mode generate grid with unique solution activate.\n");
                }
                break;
            case 'v':
                verbose = true;
                if (verbose) {
                    fprintf(stdout, "Mode verbose output activate.\n");
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

    if(unique && !generate_mode) {
        fprintf(stderr, "warning: option 'unique' conflict with solver mode,exiting!\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    if(all && generate_mode) {
        fprintf(stderr, "warning: option 'all' conflict with generate mode,exiting!\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    if(generate_mode) {
        if(output_file == NULL) {
            fprintf(stderr, "Error: In generator mode, an output file must be specified using '-o'.\n\n");
            print_usage();
            exit(EXIT_FAILURE);
        }

        if(grid_size <= 0) {
            fprintf(stderr, "Error: In generator mode, you need to ensure a grid to solve.\n\n");
            print_usage();
            exit(EXIT_FAILURE);
        }
    }

    if (output_file != NULL) {
        output_to_file(output_file, "Contenu à écrire dans le fichier de sortie");
    }

    return 0;
}