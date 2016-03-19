#include "SudokuSolver.h"

//! Function to initialize a new sudoku
/*!
 *  @param    Sudoku*         A pointer to the sudoku object to initialize
 *
 *  @returns  boolean         Returns true if successful
 *
 *  @author   Daniel Fraser   <danielfraser782@gmail.com>
 */
bool InitializeSudoku(Sudoku **sudoku)
{
    // sanity check
    if (!sudoku) {
        return false;
    }

    // allocate a new sudoku
    Sudoku *new_sudoku = (Sudoku*)calloc(1, sizeof(Sudoku));

    if (!new_sudoku) {
        return false;
    }

    // zero out the grid
    memset(new_sudoku->grid, 0, sizeof(new_sudoku->grid));

    // assign the sudoku
    *sudoku = new_sudoku;

    // success
    return true;
}

//! Function to print out the sudoku table
/*!
 *  @param    Sudoku*         A pointer to the sudoku object to print
 *
 *  @author   Daniel Fraser   <danielfraser782@gmail.com>
 */
void PrintSudoku(Sudoku *sudoku)
{
    unsigned int i = 0, j = 0;
    
    // sanity check
    if (!sudoku) {
        return;
    }

    // iterate cells and print borders/numbers
    for (i = 0; i < 9; ++i) {
        if(!i) { // top horizontal border
            printf("\n     1 2 3   4 5 6   7 8 9\n");
            printf("    _______________________\n\n");
        }

        for (j = 0; j < 9; ++j) {
            if(!j) { // left vertical border
                printf(" %d | ", i + 1);
            }

            // print current number if not 0
            if (sudoku->grid[i][j]) {
                printf("%d ", sudoku->grid[i][j]);
            } else { //otherwise place a space
                printf("  ");
            }

            // all but left vertical border
            if (((j + 1) % 3) == 0) {
                printf("| ");
            }
        }

        // all but top horizontal border
        if (((i + 1) % 3) == 0) {
            printf("\n    _______________________\n");
        }

        printf("\n");
    }
}

//! Function to place a number at a specific location in a sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to fill the number in from 0 - 8
 *  @param      unsigned int    The column to fill the number in from 0 - 8
 *  @param      unsigned int    The value to fill into the square from 1 - 9
 *
 *  @returns    boolean         Returns true if successful
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This will not do any checks if the number can be placed there, 
 *        it simply changes the value at a location
 */
bool PlaceNumber(Sudoku *sudoku, unsigned int row, unsigned int column, unsigned int value)
{
    // sanity check
    if (!sudoku 
        || column > 8
        || row > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // assign the value
    sudoku->grid[column][row] = value;

    // success
    return true;
}

//! Function to check if a specific cell is free or not
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row of the cell to check
 *  @param      unsigned int    The column of the cell to check
 *
 *  @returns    boolean         Returns true if the cell is empty
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsCellEmpty(Sudoku *sudoku, unsigned int row, unsigned int column)
{
    // sanity
    if (!sudoku) {
        return false;
    }

    // is this cell a 0?
    return (sudoku->grid[column][row] == 0);
}

//! Function to determine if a number is able to be placed at a position
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to test the number in
 *  @param      unsigned int    The column to test the number in
 *  @param      unsigned int    The number to test
 *
 *  @returns    boolean         Returns true if succesful
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool CanPlaceNumber(Sudoku *sudoku, unsigned int row, unsigned int column, unsigned int value)
{
    // sanity check
    if (!sudoku 
        || column > 8
        || row > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // make sure the cell is empty
    if (!IsCellEmpty(sudoku, row, column)) {
        return false;
    }

    // check row
    if (IsNumberInRow(sudoku, row, value)) {
        return false;
    }

    // check column
    if (IsNumberInColumn(sudoku, column, value)) {
        return false;
    }
    
    // check box
    if (IsNumberInBox(sudoku, row, column, value)) {
        return false;
    }

    // we can place a number here
    return true;
}

//! Function to determine if a number is within a specific row
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to check for the number
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsNumberInRow(Sudoku *sudoku, unsigned int row, unsigned int value)
{
    unsigned int i = 0;

    // sanity check
    if (!sudoku 
        || row > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // iterate this row
    for (i = 0; i < 9; ++i) {
        if (sudoku->grid[i][row] == value) {
            return true;
        }
    }

    return false;
}

//! Function to determine if a number is within a specific column
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The column to check for the number
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsNumberInColumn(Sudoku *sudoku, unsigned int column, unsigned int value)
{
    unsigned int i = 0;

    // sanity check
    if (!sudoku 
        || column > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // iterate this column
    for (i = 0; i < 9; ++i) {
        if (sudoku->grid[column][i] == value) {
            return true;
        }
    }

    return false;
}

//! Function to determine if a number is within a specific box
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    A row that lies within the box to check
 *  @param      unsigned int    A column that lies within the box to check
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: Specify any cell on the grid and it will determine 
 *        if the value is anywhere else in it's respective 3x3 box
 */
bool IsNumberInBox(Sudoku *sudoku, unsigned int row, unsigned int column, unsigned int value)
{
    // the top left cell of this box
    unsigned int boxrow, boxcolumn = 0, i = 0, j = 0;

    // sanity check
    if (!sudoku 
        || column > 8
        || row > 8
        || value > 9
        || value == 0) {
            return false;
    }

    
    boxrow = row - (row % 3);
    boxcolumn = column - (column % 3);

    // check this box
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (sudoku->grid[boxcolumn + i][boxrow + j] == value) {
                return true;
            }
        }
    }

    return false;
}

//! Function to determine if a row contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to check for completion
 *
 *  @returns    boolean         Returns true if the row is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsRowComplete(Sudoku *sudoku, unsigned int row)
{
    unsigned int i = 0;
    
    // sanity check
    if (!sudoku 
        || row > 8) {
            return false;
    }

    // find each number in this row
    for (i = 1; i < 10; ++i) {
        if (!IsNumberInRow(sudoku, row, i)) {
            return false;
        }
    }

    return true;
}

//! Function to determine if a column contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The column to check for completion
 *
 *  @returns    boolean         Returns true if the column is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsColumnComplete(Sudoku *sudoku, unsigned int column)
{
    unsigned int i = 0;
    
    // sanity check
    if (!sudoku 
        || column > 8) {
            return false;
    }

    // find each number in this column
    for (i = 1; i < 10; ++i) {
        if (!IsNumberInColumn(sudoku, column, i)) {
            return false;
        }
    }

    return true;
}

//! Function to determine if a 3x3 box contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    A column within the box
 *
 *  @returns    boolean         Returns true if the box is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsBoxComplete(Sudoku *sudoku, unsigned int row, unsigned int column)
{
    unsigned int i = 0;

    // sanity check
    if (!sudoku 
        || row > 8
        || column > 8) {
            return false;
    }
    
    // find each number in this box
    for (i = 1; i < 10; ++i) {
        if (!IsNumberInBox(sudoku, row, column, i)) {
            return false;
        }
    }

    return true;
}

//! Function to determine the specified sudoku contains 1 to 9 in each column, row and box
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to check for completion
 *
 *  @returns    boolean         Returns true if the sudoku is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsSudokuComplete(Sudoku *sudoku)
{
    unsigned int i = 0;
    
    // sanity check
    if (!sudoku) {
        return false;
    }

    // find each number in each row, column and box
    for (i = 0; i < 9; ++i) {
        // check each row
        if (!IsRowComplete(sudoku, i)) {
            return false;
        }

        // check each column
        if (!IsColumnComplete(sudoku, i)) {
            return false;
        }

        // check each box
        if (!IsBoxComplete(sudoku, BOXROW(i), BOXCOL(i))) {
            return false;
        }
    }

    return true;
}

//! Function which attempts to solve the boxes of the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    unsigned int    The number of cells that were correctly solved
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This function uses box checking to determine if there are any places
 *        within boxes that must be a number
 */
unsigned int SolveBoxes(Sudoku *sudoku)
{
    unsigned int b = 0, y = 0, x = 0, v = 0, xx = 0, yy = 0;
    bool onlyplace = false;
    unsigned int solvednumbers = 0;

    // sanity
    if (!sudoku) {
        return 0;
    }

    // iterate each box
    for (b = 0; b < 9; ++b) {
        // make sure this box isn't completed yet
        if (!IsBoxComplete(sudoku, BOXROW(b), BOXCOL(b))) {
            // iterate vertically
            for (y = BOXCOL(b); y < (BOXCOL(b) + 3); ++y) {
                // iterate horizontally
                for (x = BOXROW(b); x < (BOXROW(b) + 3); ++x) {
                    // if this cell is empty
                    if (IsCellEmpty(sudoku, x, y)) {
                        // iterate values
                        for (v = 1; v < 10; ++v) {
                            // if we can place a number here, check every other cell in this box
                            if (CanPlaceNumber(sudoku, x, y, v)) {
                    
                                //find out if this is the only place we can place this number
                                onlyplace = true;

                                int boxystart = (y - (y % 3));
                                int boxyend = ((y - (y % 3)) + 3);
                                int boxxstart = (x - (x % 3));
                                int boxxend = ((x - (x % 3)) + 3);

                                // iterate each cell within this box vertically
                                for (yy = boxystart; (yy < boxyend) && onlyplace; ++yy) {
                                    // iterate each cell within this box horizontally
                                    for (xx = boxxstart; (xx < boxxend) && onlyplace; ++xx) {
                                        // if this isn't the box we're originally trying to place a number in
                                        if (xx != x || yy != y) {
                                            // can we place a number in this box
                                            if (CanPlaceNumber(sudoku, xx, yy, v)) {
                                                // we can place a number in this other box, so the original choice isn't 100%
                                                onlyplace = false;
                                            }
                                        }
                                    }
                                }

                                // if we didn't discover any other places in this box that the number can be placed
                                if (onlyplace) {
                                    // put our new number here
                                    PlaceNumber(sudoku, x, y, v);
                                    solvednumbers++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // if we solved any numbers
    if(solvednumbers != 0) {
        // notify of how many numbers we solved
        printf("Box-Solved %d numbers\n", solvednumbers);
    }

    return solvednumbers;
}

//! Function which attempts to solve the rows of the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    unsigned int    The number of cells that were correctly solved
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This function uses row checking to determine if there are any places
 *        within the row that must be a number
 */
unsigned int SolveRows(Sudoku *sudoku)
{
    unsigned int y = 0, x = 0, v = 0, xx = 0;
    bool onlyplace = false;
    unsigned int solvednumbers = 0;

    // sanity
    if (!sudoku) {
        return 0;
    }

    // iterate each row
    for (y = 0; y < 9; ++y) {
        // make sure this row isn't completed yet
        if (!IsRowComplete(sudoku, y)) {
            // iterate horizontally
            for (x = 0; x < 9; ++x) {
                // if this cell is empty
                if (IsCellEmpty(sudoku, x, y)) {
                    // iterate values
                    for (v = 1; v < 10; ++v) {
                        // if we can place a number here, check every other cell in this box
                        if (CanPlaceNumber(sudoku, x, y, v)) {

                            //find out if this is the only place we can place this number
                            onlyplace = true;

                            // iterate each cell within this row horizontally
                            for (xx = 0; (xx < 9) && onlyplace; ++xx) {
                                // if this isn't the box we're originally trying to place a number in
                                if (xx != x) {
                                    // can we place a number in this box
                                    if (CanPlaceNumber(sudoku, xx, y, v)) {
                                        // we can place a number in this other box, so the original choice isn't 100%
                                        onlyplace = false;
                                    }
                                }
                            }
                            

                            // if we didn't discover any other places in this box that the number can be placed
                            if (onlyplace) {
                                // put our new number here
                                PlaceNumber(sudoku, x, y, v);
                                solvednumbers++;
                            }
                        }
                    }
                }
            }
        }
    }

    // if we solved any numbers
    if(solvednumbers != 0) {
        // notify of how many numbers we solved
        printf("Row-Solved %d numbers\n", solvednumbers);
    }

    return solvednumbers;
}

//! Function which attempts to solve the columns of the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    unsigned int    The number of cells that were correctly solved
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This function uses column checking to determine if there are any places
 *        within the columns that must be a number
 */
unsigned int SolveColumns(Sudoku *sudoku)
{
    unsigned int y = 0, x = 0, v = 0, yy = 0;
    bool onlyplace = false;
    unsigned int solvednumbers = 0;

    // sanity
    if (!sudoku) {
        return 0;
    }

    // iterate each column
    for (x = 0; x < 9; ++x) {
        // make sure this row isn't completed yet
        if (!IsColumnComplete(sudoku, x)) {
            // iterate vertically
            for (y = 0; y < 9; ++y) {
                // if this cell is empty
                if (IsCellEmpty(sudoku, x, y)) {
                    // iterate values
                    for (v = 1; v < 10; ++v) {
                        // if we can place a number here, check every other cell in this box
                        if (CanPlaceNumber(sudoku, x, y, v)) {

                            //find out if this is the only place we can place this number
                            onlyplace = true;

                            // iterate each cell within this column vertically
                            for (yy = 0; (yy < 9) && onlyplace; ++yy) {
                                // if this isn't the box we're originally trying to place a number in
                                if (yy != y) {
                                    // can we place a number in this box
                                    if (CanPlaceNumber(sudoku, x, yy, v)) {
                                        // we can place a number in this other box, so the original choice isn't 100%
                                        onlyplace = false;
                                    }
                                }
                            }
                            

                            // if we didn't discover any other places in this box that the number can be placed
                            if (onlyplace) {
                                // put our new number here
                                PlaceNumber(sudoku, x, y, v);
                                solvednumbers++;
                            }
                        }
                    }
                }
            }
        }
    }

    // if we solved any numbers
    if(solvednumbers != 0) {
        // notify of how many numbers we solved
        printf("Column-Solved %d numbers\n", solvednumbers);
    }

    return solvednumbers;
}

//! Function which attempts to solve the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    boolean         Returns true if the sudoku was successfully solved
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool SolveSudoku(Sudoku *sudoku)
{
    bool progress = true;

    // sanity
    if (!sudoku) {
        return false;
    }
    
    // loop till we can't make any more progress or the sudoku is solved
    while (progress && !IsSudokuComplete(sudoku)) {
        // start with no progress this loop
        progress = false;

        // first attempt to solve the boxes
        if (SolveBoxes(sudoku) > 0) {
            progress = true;
        }

        // now try to solve the rows
        if (SolveRows(sudoku) > 0) {
            progress = true;
        }

        // finally solve the columns
        if (SolveColumns(sudoku) > 0) {
            progress = true;
        }
    }

    // print our sudoku after all attempts to solve have been made
    PrintSudoku(sudoku);
    
    // return whether we completed or not
    return IsSudokuComplete(sudoku);
}
