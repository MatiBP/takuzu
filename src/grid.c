#include "../include/grid.h"


/*
 * Copies the content of the source grid to the destination grid.
 * Ensures that both grid pointers are valid, and their sizes match.
 * Allocates memory for the destination grid if necessary.
 *
 * Parameters:
 * - source_grid: Pointer to the source grid to copy.
 * - destination_grid: Pointer to the destination grid to copy to.
 */
void grid_copy(const t_grid* source_grid, t_grid* destination_grid) {
    // Check if the grid pointers are valid
    if (source_grid == NULL || destination_grid == NULL) {
        fprintf(stderr, "Error: Invalid grid pointers.\n");
        exit(EXIT_FAILURE);
    }

    // Check if the grid sizes match
    if (source_grid->size != destination_grid->size) {
        fprintf(stderr, "Error: Grid sizes do not match.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the destination grid if necessary
    if (destination_grid->grid == NULL) {
        destination_grid->grid = (char*)calloc(destination_grid->size * destination_grid->size, sizeof(char));
        if (destination_grid->grid == NULL) {
            fprintf(stderr, "Error: Allocation failure for destination grid.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Copy the data from the source grid to the destination grid
    memcpy(destination_grid->grid, source_grid->grid, destination_grid->size * destination_grid->size);
}


/*
 * Sets the value of the cell at coordinates (i, j) in the grid to the specified value.
 * Performs boundary checks and validation of the character value.
 *
 * Parameters:
 * - i: Row index of the cell.
 * - j: Column index of the cell.
 * - g: Pointer to the grid.
 * - v: Character value to set in the cell ('0' or '1').
 */
void set_cell(int i, int j, t_grid* g, char v) {
    // Check if coordinates are within bounds
    if (i < 0 || i >= g->size || j < 0 || j >= g->size) {
        if (option.verbose) {
            fprintf(stderr, "Warning: Coordinates (%d, %d) are out of bounds for the grid. (Function: set_cell)\n", i, j);
        }
        return;
    }

    // Check if the character value is valid
    if (v != '0' && v != '1') {
        if (option.verbose) {
            fprintf(stderr, "Warning: Invalid character '%c'. Only '0' and '1' are allowed. (Function: set_cell)\n", v);
        }
        return;
    }

    // Calculate the index corresponding to the (i, j) coordinates
    int index = i * g->size + j;
    g->grid[index] = v;
}


/*
 * Gets the value of the cell at coordinates (i, j) in the grid.
 * Performs boundary checks and returns an invalid character in case of an error.
 *
 * Parameters:
 * - i: Row index of the cell.
 * - j: Column index of the cell.
 * - g: Pointer to the grid.
 *
 * Returns:
 * The character value of the cell at the specified coordinates.
 */
char get_cell(int i, int j, t_grid* g) {
    // Check if coordinates are within bounds
    if (i < 0 || i >= g->size || j < 0 || j >= g->size) {
        if (option.verbose) {
            fprintf(stderr, "Warning: Coordinates (%d, %d) are out of bounds for the grid. (Function: get_cell)\n", i, j);
        }
        // Return an invalid character to indicate an error
        return ' ';
    }

    // Calculate the index corresponding to the (i, j) coordinates
    int index = i * g->size + j;
    return g->grid[index];
}


/*
 * Checks the consistency of the Takuzu grid.
 * Returns true if the grid is consistent; otherwise, returns false.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is consistent; otherwise, false.
 */
bool is_consistent(t_grid* g) {
    // Check for identical rows or columns
    if (!check_same_col_or_row(g)) {
        if (option.verbose) {
            fprintf(stderr, "Warning: Invalid same col or row. (Function: is_consistent)\n");
        }
        return false;
    }

    // Check for the correct number of zeros and ones
    if (!check_number_of_zeros_ones(g)) {
        if (option.verbose) {
            fprintf(stderr, "Warning: Invalid number of zeros or ones. (Function: is_consistent)\n");
        }
        return false;
    }

    // Check for consecutive zeros and ones in rows
    for (int row = 0; row < g->size; row++) {
        if (!check_consecutive_zeros_ones(row, g, true)) {
            if (option.verbose) {
                fprintf(stderr, "Warning: Invalid consecutive zeros or ones in row %d. (Function: is_consistent)\n", row);
            }
            return false;
        }
    }

    // Check for consecutive zeros and ones in columns
    for (int col = 0; col < g->size; col++) {
        if (!check_consecutive_zeros_ones(col, g, false)) {
            if (option.verbose) {
                fprintf(stderr, "Warning: Invalid consecutive zeros or ones in column %d. (Function: is_consistent)\n", col);
            }
            return false;
        }
    }

    return true;
}


/**
 * Checks if there are any identical rows or columns in the given Takuzu grid.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * true if no identical rows or columns are found, false otherwise.
 */
bool check_same_col_or_row(t_grid* g) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in check_same_col_or_row.\n");
        return false;
    }
    bool foundIdentical = false;

    // Check for identical rows
    for (int row1 = 0; row1 < g->size; row1++) {
        for (int row2 = row1 + 1; row2 < g->size; row2++) {
            if (are_rows_identical(row1, row2, g)) {
                if (option.verbose) {
                    fprintf(stderr, "Warning: Identical rows found: %d %d\n", row1, row2);
                }
                foundIdentical = true;
            }
        }
    }

    // Check for identical columns
    for (int col1 = 0; col1 < g->size; col1++) {
        for (int col2 = col1 + 1; col2 < g->size; col2++) {
            if (are_columns_identical(col1, col2, g)) {
                if (option.verbose) {
                    fprintf(stderr, "Warning: Identical columns found: %d %d\n", col1, col2);
                }
                foundIdentical = true;
            }
        }
    }

    // Return false only if at least one pair of identical rows or columns is found
    return !foundIdentical;
}


/*
 * Checks if two rows in the Takuzu grid are identical.
 * Returns true if the rows are identical; otherwise, returns false.
 *
 * Parameters:
 * - row1: Index of the first row.
 * - row2: Index of the second row.
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the rows are identical; otherwise, false.
 */
bool are_rows_identical(int row1, int row2, t_grid* g) {
    for (int col = 0; col < g->size; col++) {
        if (get_cell(row1, col, g) != get_cell(row2, col, g) || get_cell(row1, col, g) == '_') {
            return false;
        }
    }
    return true;
}


/*
 * Checks if two columns in the Takuzu grid are identical.
 * Returns true if the columns are identical; otherwise, returns false.
 *
 * Parameters:
 * - col1: Index of the first column to compare.
 * - col2: Index of the second column to compare.
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the columns are identical; otherwise, false.
 */
bool are_columns_identical(int col1, int col2, t_grid* g) {
    for (int row = 0; row < g->size; row++) {
        if (get_cell(row, col1, g) != get_cell(row, col2, g) || get_cell(row, col1, g) == '_') {
            return false;
        }
    }
    return true;
}


/*
 * Checks for consecutive occurrences of '0' or '1' in a row or column of the Takuzu grid.
 * Returns true if no more than two consecutive '0' or '1' are found; otherwise, returns false.
 *
 * Parameters:
 * - index: Index of the row or column to check for consecutive occurrences.
 * - g: Pointer to the Takuzu grid.
 * - is_row: Boolean indicating whether to check a row (true) or column (false).
 *
 * Returns:
 * True if no more than two consecutive '0' or '1' are found; otherwise, false.
 * If the grid pointer is NULL, an error message is displayed, and false is returned.
 * Displays warnings if more than two consecutive '0' or '1' are found in the grid.
 */
bool check_consecutive_zeros_ones(int index, t_grid* g, bool is_row) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in check_consecutive_zeros_ones.\n");
        return false;
    }
    int consecutive_zeros = 0;
    int consecutive_ones = 0;

    for (int i = 0; i < g->size; i++) {
        char current = is_row ? get_cell(index, i, g) : get_cell(i, index, g);

        switch (current) {
        case '0':
            consecutive_zeros++;
            consecutive_ones = 0;
            break;
        case '1':
            consecutive_ones++;
            consecutive_zeros = 0;
            break;
        default:
            consecutive_zeros = 0;
            consecutive_ones = 0;
            break;
        }

        if (consecutive_zeros > 2 || consecutive_ones > 2) {
            return false;
        }
    }
    return true;
}


/*
 * Checks the number of '0' and '1' in each row and column of the Takuzu grid.
 * Returns true if the number of '0' and '1' is consistent; otherwise, returns false.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the number of '0's and '1's is consistent; otherwise, false.
 * If the grid pointer is NULL, an error message is displayed, and false is returned.
 * Displays warnings if the number of '0's or '1's exceeds half of the row or column size.
 */
bool check_number_of_zeros_ones(t_grid* g) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in check_number_of_zeros_ones.\n");
        return false;
    }

    int size = g->size;

    // Check the number of '0's and '1's in each row
    for (int row = 0; row < size; row++) {
        int zeroCount = 0;
        int oneCount = 0;

        for (int col = 0; col < size; col++) {
            char cell = get_cell(row, col, g);

            if (cell == '0') {
                zeroCount++;
            }
            else if (cell == '1') {
                oneCount++;
            }
        }

        if (zeroCount > size / 2 || oneCount > size / 2) {
            return false;
        }
    }

    // Check the number of '0's and '1's in each column
    for (int col = 0; col < size; col++) {
        int zeroCount = 0;
        int oneCount = 0;

        for (int row = 0; row < size; row++) {
            char cell = get_cell(row, col, g);

            if (cell == '0') {
                zeroCount++;
            }
            else if (cell == '1') {
                oneCount++;
            }
        }

        if (zeroCount > size / 2 || oneCount > size / 2) {
            return false;
        }
    }

    return true;
}


/*
 * Checks the validity of the Takuzu grid.
 * Returns true if the grid is valid; otherwise, returns false.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is valid; otherwise, false.
 * If the grid is inconsistent, an error message is displayed (if option.verbose is enabled), and false is returned.
 * Displays a warning (if option.verbose is enabled) if an empty cell is found, and false is returned.
 */
bool is_valid(t_grid* g) {
    if (!is_consistent(g)) {
        if (option.verbose) {
            fprintf(stderr, "Error: Grid is inconsistent.\n");
        }
        return false;
    }

    // Check if there are any empty cells
    for (int row = 0; row < g->size; row++) {
        for (int col = 0; col < g->size; col++) {
            char cell = get_cell(row, col, g);
            if (cell != '0' && cell != '1') {
                if (option.verbose) {
                    fprintf(stderr, "Warning: Found an empty cell at (%d, %d).\n", row, col);
                }
                return false;
            }
        }
    }

    return true;
}


/*
 * Tries to fill the grid by placing '1' or '0' based on consecutive '0's or '1's in rows.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is changed by the heuristic; otherwise, false.
 * Displays a warning if the grid pointer is NULL and returns false.
 */
bool apply_consecutive_zeros_ones_rows(t_grid* g) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in applyConsecutiveZerosOnesRows.\n");
        return false;
    }
    bool gridChanged = false;

    for (int row = 0; row < g->size; row++) {
        for (int col = 0; col < g->size - 2; col++) {
            // Search for two consecutive zeros in the row
            if (get_cell(row, col, g) == '0' && get_cell(row, col + 1, g) == '0') {
                // Check the possibility of placing '1' at the third position
                if (col + 2 < g->size && get_cell(row, col + 2, g) == '_') {
                    set_cell(row, col + 2, g, '1');
                    gridChanged = true;
                }
                // If the third position is outside the grid, try adding '1' at the first available position before the zeros
                if (col + 2 >= g->size && col - 1 >= 0 && get_cell(row, col - 1, g) == '_') {
                    set_cell(row, col - 1, g, '1');
                    gridChanged = true;
                }
            }
            // Search for two consecutive ones in the row
            if (get_cell(row, col, g) == '1' && get_cell(row, col + 1, g) == '1') {
                // Check the possibility of placing '0' at the third position
                if (col + 2 < g->size && get_cell(row, col + 2, g) == '_') {
                    set_cell(row, col + 2, g, '0');
                    gridChanged = true;
                }
                // If the third position is outside the grid, try adding '0' at the first available position before the ones
                if (col + 2 >= g->size && col - 1 >= 0 && get_cell(row, col - 1, g) == '_') {
                    set_cell(row, col - 1, g, '0');
                    gridChanged = true;
                }
            }
        }
    }

    return gridChanged;
}


/*
 * Attempts to fill empty cells based on the adjacent values in rows and columns.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is modified by the heuristic; otherwise, false.
 * Displays an error message (if option.verbose is enabled) in case of a NULL grid pointer.
 */
bool middle_pattern_heuristic(t_grid* g) {
    if (g == NULL) {
        if (option.verbose) {
            fprintf(stderr, "Error: Grid is NULL in middle_pattern_heuristic.\n");
        }
        return false;
    }

    bool state = false;

    for (int i = 0; i < g->size; i++) {
        for (int j = 1; j < g->size; j++) {
            // Row part
            if (get_cell(i, j, g) == '_') {
                if (get_cell(i, j - 1, g) == '0' && get_cell(i, j + 1, g) == '0') {
                    set_cell(i, j, g, '1');
                    state = true;
                }

                if (get_cell(i, j - 1, g) == '1' && get_cell(i, j + 1, g) == '1') {
                    set_cell(i, j, g, '0');
                    state = true;
                }
            }
            // Column part
            if (get_cell(j, i, g) == '_') {
                if (get_cell(j - 1, i, g) == '0' && get_cell(j + 1, i, g) == '0') {
                    set_cell(j, i, g, '1');
                    state = true;
                }

                if (get_cell(j - 1, i, g) == '1' && get_cell(j + 1, i, g) == '1') {
                    set_cell(j, i, g, '0');
                    state = true;
                }
            }
        }
    }
    return state;
}


/*
 * Attempts to fill empty cells based on consecutive values in columns.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is modified by the heuristic; otherwise, false.
 * Displays an error message (if option.verbose is enabled) in case of a NULL grid pointer.
 */
bool apply_consecutive_zeros_ones_columns(t_grid* g) {
    if (g == NULL) {
        if (option.verbose) {
            fprintf(stderr, "Error: Grid is NULL in applyConsecutiveZerosOnesColumns.\n");
        }
        return false;
    }
    bool gridChanged = false;

    for (int col = 0; col < g->size; col++) {
        for (int row = 0; row < g->size - 2; row++) {
            if (get_cell(row, col, g) == '0' && get_cell(row + 1, col, g) == '0') {
                if (get_cell(row + 2, col, g) == '_') {
                    set_cell(row + 2, col, g, '1');
                    gridChanged = true;
                }
                if (get_cell(row - 1, col, g) == '_') {
                    set_cell(row - 1, col, g, '1');
                    gridChanged = true;
                }
            }
            if (get_cell(row, col, g) == '1' && get_cell(row + 1, col, g) == '1') {
                if (get_cell(row + 2, col, g) == '_') {
                    set_cell(row + 2, col, g, '0');
                    gridChanged = true;
                }
                if (get_cell(row - 1, col, g) == '_') {
                    set_cell(row - 1, col, g, '0');
                    gridChanged = true;
                }
            }
        }
    }

    return gridChanged;
}


/*
 * Fills empty cells in rows where the count of '0' is half of the grid size.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is modified by the heuristic; otherwise, false.
 * Displays an error message (if option.verbose is enabled) in case of a NULL grid pointer.
 */
bool apply_all_zeros_filled_rows(t_grid* g) {
    if (g == NULL) {
        if (option.verbose) {
            fprintf(stderr, "Error: Grid is NULL in applyAllZerosFilledRows.\n");
        }
        return false;
    }
    bool gridChanged = false;

    for (int row = 0; row < g->size; row++) {
        int zeroCount = 0;
        for (int col = 0; col < g->size; col++) {
            if (get_cell(row, col, g) == '0') {
                zeroCount++;
            }
        }

        if (zeroCount == g->size / 2) {
            for (int col = 0; col < g->size; col++) {
                if (get_cell(row, col, g) == '_') {
                    set_cell(row, col, g, '1');
                    gridChanged = true;
                }
            }
        }
    }

    return gridChanged;
}


/*
 * Fills empty cells in columns where the count of '0' is half of the grid size.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is modified by the heuristic; otherwise, false.
 * Displays an error message (if option.verbose is enabled) in case of a NULL grid pointer.
 */
bool apply_all_zeros_filled_columns(t_grid* g) {
    if (g == NULL) {
        if (option.verbose) {
            fprintf(stderr, "Error: Grid is NULL in applyAllZerosFilledColumns.\n");
        }
        return false;
    }
    bool gridChanged = false;

    for (int col = 0; col < g->size; col++) {
        int zeroCount = 0;

        for (int row = 0; row < g->size; row++) {
            if (get_cell(row, col, g) == '0') {
                zeroCount++;
            }
        }

        if (zeroCount == g->size / 2) {
            for (int row = 0; row < g->size; row++) {
                if (get_cell(row, col, g) == '_') {
                    set_cell(row, col, g, '1');
                    gridChanged = true;
                }
            }
        }
    }

    return gridChanged;
}


/*
 * Fills empty cells in rows where the count of '1' is half of the grid size.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is modified by the heuristic; otherwise, false.
 * Displays an error message (if option.verbose is enabled) in case of a NULL grid pointer.
 */
bool apply_all_ones_filled_rows(t_grid* g) {
    if (g == NULL) {
        if (option.verbose) {
            fprintf(stderr, "Error: Grid is NULL in applyAllOnesFilledRows.\n");
        }
        return false;
    }
    bool gridChanged = false;

    for (int row = 0; row < g->size; row++) {
        int oneCount = 0;

        for (int col = 0; col < g->size; col++) {
            if (get_cell(row, col, g) == '1') {
                oneCount++;
            }
        }

        if (oneCount == g->size / 2) {
            for (int col = 0; col < g->size; col++) {
                if (get_cell(row, col, g) == '_') {
                    set_cell(row, col, g, '0');
                    gridChanged = true;
                }
            }
        }
    }

    return gridChanged;
}


/*
 * Fills empty cells in columns where the count of '1' is half of the grid size.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Returns:
 * True if the grid is modified by the heuristic; otherwise, false.
 * Displays an error message (if option.verbose is enabled) in case of a NULL grid pointer.
 */
bool apply_all_ones_filled_columns(t_grid* g) {
    if (g == NULL) {
        if (option.verbose) {
            fprintf(stderr, "Error: Grid is NULL in applyAllOnesFilledColumns.\n");
        }
        return false;
    }
    bool gridChanged = false;

    for (int col = 0; col < g->size; col++) {
        int oneCount = 0;

        for (int row = 0; row < g->size; row++) {
            if (get_cell(row, col, g) == '1') {
                oneCount++;
            }
        }

        if (oneCount == g->size / 2) {
            for (int row = 0; row < g->size; row++) {
                if (get_cell(row, col, g) == '_') {
                    set_cell(row, col, g, '0');
                    gridChanged = true;
                }
            }
        }
    }

    return gridChanged;
}


/*
 * Applies various heuristics to the Takuzu grid until stability is reached.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 *
 * Note:
 * The function applies heuristics in a potentially optimized order to improve efficiency.
 * In case of errors or inconsistencies, appropriate error messages are displayed.
 */
void apply_heuristics_until_stable(t_grid* g) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in apply_heuristics_until_stable.\n");
        return;
    }

    // Check if the initial grid is consistent
    if (!is_consistent(g)) {
        fprintf(stderr, "Error: Inconsistent grid. No need to try solving it!\n");
        return;
    }

    bool gridChanged = true;
    while (gridChanged) {
        gridChanged = false;

        // Apply various heuristics in a potentially optimized order
        if (apply_all_zeros_filled_rows(g)) {
            gridChanged = true;
        }

        if (apply_all_zeros_filled_columns(g)) {
            gridChanged = true;
        }

        if (apply_all_ones_filled_rows(g)) {
            gridChanged = true;
        }

        if (apply_all_ones_filled_columns(g)) {
            gridChanged = true;
        }

        if (apply_consecutive_zeros_ones_rows(g)) {
            gridChanged = true;
        }

        if (apply_consecutive_zeros_ones_columns(g)) {
            gridChanged = true;
        }

        if (middle_pattern_heuristic(g)) {
            gridChanged = true;
        }
    }
}


/*
 * Checks the consistency of the Takuzu grid after placing a cell at the specified coordinates.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 * - row: Row index of the cell placement.
 * - col: Column index of the cell placement.
 * - cell_value: Value to be placed at the specified coordinates.
 *
 * Returns:
 * True if the grid remains consistent after the placement; otherwise, false.
 * If the grid is inconsistent or there is an error, a message is displayed.
 */
bool check_consistency_after_placement(t_grid* g, int row, int col, char cell_value) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in check_consistency_after_placement.\n");
        return false;
    }

    // Allocate and copy the current grid to avoid modification
    t_grid gd;
    grid_allocate(&gd, g->size);
    grid_copy(g, &gd);

    // Modify the copy with the new value
    set_cell(row, col, &gd, cell_value);

    // Check the consistency of the copy
    bool consistency = is_consistent(&gd);

    // Free the allocated memory for the copy
    grid_free(&gd);

    // Display a message if the grid is inconsistent or an error occurs
    if (!consistency && option.verbose) {
        fprintf(stderr, "Warning: Placement of cell at (%d, %d) with value '%c' resulted in inconsistency.\n", row, col, cell_value);
    }

    return consistency;
}


/*
 * Places a cell strategically at the specified coordinates in the Takuzu grid.
 * Chooses between '0' and '1' based on consistency checks after placement.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid.
 * - row: Row index for cell placement.
 * - col: Column index for cell placement.
 *
 * Returns:
 * '0', '1', or '_' based on the strategic placement choice.
 * If an error occurs, or the grid is inconsistent, a message may be displayed.
 */
char place_cell_strategically(t_grid* g, int row, int col) {
    // Check for NULL grid
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in place_cell_strategically.\n");
        return '_';
    }

    // Check if placing a '0' is consistent
    bool is_zero_consistent = check_consistency_after_placement(g, row, col, '0');

    // Check if placing a '1' is consistent
    bool is_one_consistent = check_consistency_after_placement(g, row, col, '1');

    // If both '0' and '1' placements are inconsistent, do nothing
    if (!is_zero_consistent && !is_one_consistent) {
        return '_';
    }

    // If both '0' and '1' placements are consistent, choose randomly between '0' and '1'
    if (is_zero_consistent && is_one_consistent) {
        return (rand() % 2 == 0) ? '0' : '1';
    }

    // If placing '0' is consistent, choose '0'
    if (is_zero_consistent) {
        return '0';
    }

    // If placing '1' is consistent, choose '1'
    if (is_one_consistent) {
        return '1';
    }

    // Default case, choose randomly between '0' and '1'
    return '_';
}


/*
 * Generates a random Takuzu grid with a specified percentage of filled cells.
 *
 * Parameters:
 * - g: Pointer to the Takuzu grid to be generated.
 * - percentage: Percentage of cells to be filled with '0' or '1'.
 *
 * Returns:
 * The generated Takuzu grid.
 * If an error occurs, or the grid pointer is NULL, an error message may be displayed.
 */
void generate_random_grid(t_grid* g, int percentage) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in generate_random_grid.\n");
        return;
    }

    // Initialize random number generator
    srand(time(NULL));

    // Check if the percentage is valid
    if (percentage < 0 || percentage > 100) {
        fprintf(stderr, "Error: Invalid percentage value\n");
        exit(EXIT_FAILURE);
    }

    grid_free(g);
    grid_allocate(g, g->size);


    int num_cells_to_fill = (percentage * g->size * g->size) / 100;

    // Randomly place '0' and '1' cells
    while (num_cells_to_fill > 0) {
        int row = rand() % g->size;
        int col = rand() % g->size;

        if (get_cell(row, col, g) == '_') {
            char cell_value = place_cell_strategically(g, row, col);
            set_cell(row, col, g, cell_value);
            num_cells_to_fill--;
        }
    }
}


void generate_random_grid_with_solution(t_grid* g, int percentage) {
    if (g == NULL) {
        fprintf(stderr, "Error: Grid is NULL in generate_random_grid_with_solution.\n");
        return;
    }


    if (g->size == 8 || g->size == 4) { //We only do that for grid 4 or 8 because it's too long for 16,32 or 64

        // Max try
        int max_attempts = INT_MAX;
        int attempt_count = 0;

        t_grid* solution = NULL;

        do {
            generate_random_grid(g, percentage);

            // Verify if the grid have one solution
            solution = grid_solver(g, MODE_FIRST);

            grid_free(solution);

            attempt_count++;

        } while (solution == NULL && attempt_count < max_attempts);

        if (solution == NULL) {
            fprintf(stderr, "Error: Unable to generate a grid with at least one solution.\n");
        }
    }
    else {
        generate_random_grid(g, percentage);

    }
}



/*
 * Applies a choice to the Takuzu grid by setting the specified cell to the chosen value.
 *
 * Parameters:
 * - grid: Pointer to the Takuzu grid.
 * - choice: The choice containing row, column, and the chosen value.
 *
 * Returns:
 * This function modifies the grid in place.
 * If an error occurs (e.g., NULL grid or invalid choice coordinates), an error message may be displayed.
 */
void grid_choice_apply(t_grid* grid, const choice_t choice) {
    if (grid == NULL) {
        fprintf(stderr, "Error: Grid is NULL in grid_choice_apply.\n");
        return;
    }

    // Ensure the choice is within valid bounds
    if (choice.row < 0 || choice.row >= grid->size ||
        choice.column < 0 || choice.column >= grid->size) {
        fprintf(stderr, "Error: Invalid choice coordinates.\n");
        return;
    }

    // Set the chosen cell to the specified value
    set_cell(choice.row, choice.column, grid, choice.choice);
}


/*
 * Prints the details of a choice made on a given file, for debugging purposes.
 *
 * Parameters:
 * - choice: The choice to print.
 * - fd: File descriptor (e.g., stdout, stderr, or a file) where the choice details will be printed.
 *
 * Returns:
 * This function prints the choice details to the specified file descriptor.
 */
void grid_choice_print(const choice_t choice, FILE* fd) {
    // Check for NULL file descriptor
    if (fd == NULL) {
        fprintf(stderr, "Error: NULL file descriptor in grid_choice_print.\n");
        return;
    }

    fprintf(fd, "Choice Details: Row = %d, Column = %d, Value = '%c'\n", choice.row, choice.column, choice.choice);
}


/*
## Attempt to Optimize Grid Choice Strategies more deterministic ##
#### Note: Not working propely ####
*/

// Helper function to count the number of zeros and ones in a row
// Returns the count of specified value in the given row of the grid.
int count_empty_zeros_ones_in_row(int row, t_grid* grid, char value) {
    if (grid == NULL) {
        fprintf(stderr, "Error: Grid is NULL in count_empty_zeros_ones_in_row.\n");
        return false;
    }
    int count = 0;
    for (int col = 0; col < grid->size; col++) {
        if (get_cell(row, col, grid) == value) {
            count++;
        }
    }
    return count;
}


// Helper function to count the number of zeros and ones in a column
// Returns the count of specified value in the given column of the grid.
int count_empty_zeros_ones_in_column(int col, t_grid* grid, char value) {
    if (grid == NULL) {
        fprintf(stderr, "Error: Grid is NULL in count_empty_zeros_ones_in_column.\n");
        return false;
    }
    int count = 0;
    for (int row = 0; row < grid->size; row++) {
        if (get_cell(row, col, grid) == value) {
            count++;
        }
    }
    return count;
}


// Helper function to count the number of zeros and ones in a square
// Returns the count of specified value in the given square of the grid.
int count_zeros_ones_in_square(int start_row, int start_col, t_grid* grid, char value) {
    if (grid == NULL) {
        fprintf(stderr, "Error: Grid is NULL in count_zeros_ones_in_square.\n");
        return false;
    }
    int count = 0;
    int square_size = (int)sqrt(grid->size);
    for (int row = start_row; row < start_row + square_size; row++) {
        for (int col = start_col; col < start_col + square_size; col++) {
            if (get_cell(row, col, grid) == value) {
                count++;
            }
        }
    }
    return count;
}


// Helper function to get the number of choices for a cell
// Returns the total count of zeros and ones in the row, column, and square of the specified cell.
int count_choices_for_cell(int row, int col, t_grid* grid) {
    if (grid == NULL) {
        fprintf(stderr, "Error: Grid is NULL in count_choices_for_cell.\n");
        return false;
    }
    if (get_cell(row, col, grid) != '_') {
        return 0;  // Cell is already filled
    }

    int zeros_in_row = count_empty_zeros_ones_in_row(row, grid, '0');
    int ones_in_row = count_empty_zeros_ones_in_row(row, grid, '1');
    int zeros_in_col = count_empty_zeros_ones_in_column(col, grid, '0');
    int ones_in_col = count_empty_zeros_ones_in_column(col, grid, '1');

    int square_size = (int)sqrt(grid->size);
    int start_row = (row / square_size) * square_size;
    int start_col = (col / square_size) * square_size;
    int zeros_in_square = count_zeros_ones_in_square(start_row, start_col, grid, '0');
    int ones_in_square = count_zeros_ones_in_square(start_row, start_col, grid, '1');

    int total_zeros = zeros_in_row + zeros_in_col + zeros_in_square;
    int total_ones = ones_in_row + ones_in_col + ones_in_square;

    return (total_zeros + total_ones);
}


// Improved grid_choice_deterministic function
// Returns the best choice based on the total number of zeros and ones in the grid.
choice_t grid_choice_deterministic(t_grid* grid, char choice) {
    choice_t best_choice;
    best_choice.row = -1;
    best_choice.column = -1;
    best_choice.choice = choice;

    int min_choices = INT_MAX;

    for (int row = 0; row < grid->size; row++) {
        for (int col = 0; col < grid->size; col++) {
            if (get_cell(row, col, grid) == '_') {
                int choices = count_choices_for_cell(row, col, grid);
                if (choices < min_choices) {
                    min_choices = choices;
                    best_choice.row = row;
                    best_choice.column = col;
                }
            }
        }
    }

    return best_choice;
}

/* #### END #### */


/*
 * Finds and returns an ordered choice to fill an empty cell in the grid with a specified value.
 *
 * Parameters:
 * - grid: Pointer to the Takuzu grid.
 * - choice: The specified value for the choice.
 *
 * Returns:
 * This function returns a choice_t structure representing the ordered choice to fill an empty cell.
 * If the grid is NULL or if there are no empty cells, they returned invalid coordinates.
 */
choice_t grid_choice_ordered(t_grid* grid, char choice) {
    // Initialize the choice structure with default values
    choice_t best_choice;
    best_choice.row = -1;
    best_choice.column = -1;
    best_choice.choice = choice;
    if (grid == NULL) {
        fprintf(stderr, "Error: Grid is NULL in grid_choice_ordered.\n");
        return best_choice;
    }

    // Traverse the grid to find the first empty cell
    for (int row = 0; row < grid->size; row++) {
        for (int col = 0; col < grid->size; col++) {
            if (get_cell(row, col, grid) == '_') {
                best_choice.row = row;
                best_choice.column = col;
                // Stop traversal once the first empty cell is found
                return best_choice;
            }
        }
    }

    return best_choice;
}


/*
 * Attempts to place a choice in the Takuzu grid.
 * Returns true if successful; otherwise, returns false.
 *
 * Parameters:
 * - grid: Pointer to the Takuzu grid.
 * - choice: The preferred choice ('0' or '1') to be placed in an empty cell.
 *
 * Returns:
 * True if the choice is successfully placed; otherwise, false.
 *
 * The function finds the best empty cell for the preferred choice using
 * grid_choice_ordered, checks the consistency after placement, and applies
 * the choice if consistent. Returns true on success, indicating a valid choice,
 * and false otherwise.
 */
bool grid_choice(t_grid* grid, char choice) {
    bool we_make_choice = false;

    choice_t best_choice = grid_choice_ordered(grid, choice);
    if (check_consistency_after_placement(grid, best_choice.row, best_choice.column, best_choice.choice)) {
        grid_choice_apply(grid, best_choice);
        return true;
    }
    return we_make_choice;
}


t_grid* grid_solver_backtracking(t_grid* grid, mode_t mode, int* solution_count) {
    if (grid == NULL) {
        fprintf(stderr, "Error: Grid is NULL in grid_solver_backtracking.\n");
        return NULL;
    }

    if (is_valid(grid)) {
        t_grid* solution = malloc(sizeof(t_grid));
        if (solution == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for solution.\n");
            exit(EXIT_FAILURE);
        }
        grid_allocate(solution, grid->size);
        grid_copy(grid, solution);
        (*solution_count)++;
        return solution;  // Return the solved grid
    }

    if (!is_consistent(grid)) {
        if (option.verbose) {
            printf("The grid is inconsistent.\n");
        }
        return NULL;
    }

    // Application of heuristics
    apply_heuristics_until_stable(grid);

    if (is_valid(grid)) {
        t_grid* solution = malloc(sizeof(t_grid));
        if (solution == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for solution.\n");
            exit(EXIT_FAILURE);
        }
        grid_allocate(solution, grid->size);
        grid_copy(grid, solution);
        (*solution_count)++;
        return solution;  // Return the solved grid
    }

    // If the grid is inconsistent after applying heuristics, return NULL
    if (!is_consistent(grid)) {
        if (option.verbose) {
            printf("The grid is inconsistent.\n");
        }
        return NULL;
    }

    t_grid original_grid;
    grid_allocate(&original_grid, grid->size);
    grid_copy(grid, &original_grid);

    for (char choice = '0'; choice <= '1'; choice++) {
        if (grid_choice(grid, choice)) {
            t_grid* solution = grid_solver_backtracking(grid, mode, solution_count);
            if (solution != NULL) {
                // Return the solved grid
                return solution;
            }

            // Backtracking must restore the original state
            grid_copy(&original_grid, grid);
        }
    }

    // Cleanup
    grid_free(&original_grid);
    return NULL;
}



void find_first_solution(t_grid* grid, const mode_t mode) {
    int solution_count = 0;
    t_grid* solution = grid_solver_backtracking(grid, mode, &solution_count);

    if (solution != NULL) {
        // Print the number of solutions
        printf("Number of solutions: %d\n", solution_count);

        // Print the first solution
        printf("Solution 1\n");

        // Cleanup
        grid_free(solution);
    }
    else {
        printf("No solution found.\n");
    }
}



void find_all_solutions(t_grid* grid, const mode_t mode) {
    int solution_count = 0;

    // Reset the grid to its original state for further use if needed
    t_grid original_grid;
    grid_allocate(&original_grid, grid->size);
    grid_copy(grid, &original_grid);

    // Array to store solutions
    t_grid* solutions[INT_MAX];

    while (1) {
        t_grid* solution = grid_solver_backtracking(grid, mode, &solution_count);
        if (solution == NULL) {
            break;  // No more solutions
        }

        solutions[solution_count - 1] = solution;
    }

    // Print the number of solutions
    printf("Number of solutions: %d\n", solution_count);

    // Print each solution
    for (int i = 0; i < solution_count; ++i) {
        printf("Solution %d\n", i + 1);
        grid_print(solutions[i], stdout);
        grid_free(solutions[i]);  // Free each solution
    }

    // Cleanup
    grid_free(&original_grid);
}





t_grid* grid_solver(t_grid* grid, const mode_t mode) {
    // Make a copy of the original grid to preserve the input
    t_grid original_grid;
    grid_allocate(&original_grid, grid->size);
    grid_copy(grid, &original_grid);

    // Call the solving function based on the mode
    if (mode == MODE_FIRST) {
        find_first_solution(grid, mode);
    }
    else if (mode == MODE_ALL) {
        find_all_solutions(grid, mode);
    }

    // No need to return the modified grid since it's modified in place
    // Return NULL or the original grid based on your needs
    grid_free(&original_grid);
    return grid;  // Returning the original grid or NULL based on your program logic
}
