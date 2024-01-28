#ifndef GRID_H
#define GRID_H

#include "../include/takuzu.h"

// Structure to represent a choice in the grid
typedef struct {
    int row;
    int column;
    char choice;  // '0' or '1' to represent choices
} choice_t;

// Function or prototypes for grid operations

// Grid copying functions
void grid_copy(const t_grid* source_grid, t_grid* destination_grid);

// Cell manipulation functions
void set_cell(int i, int j, t_grid* g, char v);
char get_cell(int i, int j, t_grid* g);

// Consistency checking functions
bool is_consistent(t_grid* g);
bool check_same_col_or_row(t_grid* g);
bool are_rows_identical(int row1, int row2, t_grid* g);
bool are_columns_identical(int col1, int col2, t_grid* g);
bool check_consecutive_zeros_ones(int index, t_grid* g, bool is_row);
bool check_number_of_zeros_ones(t_grid* g);
bool is_valid(t_grid* g);

// Heuristics functions
bool apply_consecutive_zeros_ones_rows(t_grid* g);
bool middle_pattern_heuristic(t_grid* g);
bool apply_consecutive_zeros_ones_columns(t_grid* g);
bool apply_all_zeros_filled_rows(t_grid* g);
bool apply_all_zeros_filled_columns(t_grid* g);
bool apply_all_ones_filled_rows(t_grid* g);
bool apply_all_ones_filled_columns(t_grid* g);
void apply_heuristics_until_stable(t_grid* g);

// Grid generation functions, and check the consistency after a choice
bool check_consistency_after_placement(t_grid* g, int row, int col, char cell_value);
char place_cell_strategically(t_grid* g, int row, int col);
void generate_random_grid(t_grid* g, int percentage);
void generate_random_grid_with_solution(t_grid* g, int percentage);

// Grid choice functions
void grid_choice_apply(t_grid* grid, choice_t choice);
void grid_choice_print(const choice_t choice, FILE* fd);

int count_empty_zeros_ones_in_row(int row, t_grid* grid, char value);
int count_empty_zeros_ones_in_column(int col, t_grid* grid, char value);
int count_zeros_ones_in_square(int start_row, int start_col, t_grid* grid, char value);
int count_choices_for_cell(int row, int col, t_grid* grid);
choice_t grid_choice_deterministic(t_grid* grid, char choice);

choice_t grid_choice_ordered(t_grid* grid, char choice);
bool grid_choice(t_grid* grid, char choice);

// Backtracking functions
t_grid* grid_solver_backtracking(t_grid* grid, mode_t mode, int* solution_count);
void find_first_solution(t_grid* grid, const mode_t mode);
void find_all_solutions(t_grid* grid, const mode_t mode);
t_grid* grid_solver(t_grid* grid, const mode_t mode);

#endif // GRID_H