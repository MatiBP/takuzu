#ifndef TAKUZU_H
#define TAKUZU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h> 
#include <time.h>
#include <limits.h>
#include <math.h>

#define MAX_GRID_SIZE 5000 //A grid have a max size of 64

typedef struct {
    int size;
    char* grid;
} t_grid;

typedef enum {
    MODE_FIRST,
    MODE_ALL
} mode_t;

typedef struct {
    bool verbose;
    bool unique;
    char* output_file;
    bool all;
    bool generate_mode;
    int number;
    int grid_size;
    mode_t mode;
} takuzu_Options;

// External declaration of the 'option' variable
extern takuzu_Options option;

// Function to initialize Takuzu options
void initializeTakuzuOptions(takuzu_Options* options);

void output_to_file(const char* filename, const char* content);

// Function to allocate memory for the grid
void grid_allocate(t_grid* g, int size);

// Function to free memory allocated for the grid
void grid_free(t_grid* g);

// Function to print the grid to a file
void grid_print(t_grid* g, FILE* fd);

// Function to check if a character is valid for the Takuzu grid
bool check_char(const char c);

// Function to parse a File to Takuzu grid
int file_parser(t_grid* grid, const char* filename);

#endif /* TAKUZU_H */