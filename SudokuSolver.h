#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Converts boxes 0 - 8 from left to right, top to bottom. into their top left square x/y
/* 
 * 0 1 2
 * 3 4 5
 * 6 7 8
 */
#define BOXROW(x)  ((x % 3) * 3)
#define BOXCOL(x)  (x - (x % 3))

typedef struct
{
    // the grid itself
    unsigned int grid[9][9];
} Sudoku;

//! Function to initialize a new sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to initialize
 *
 *  @returns    boolean         Returns true if successful
 */
bool InitializeSudoku(Sudoku **);

//! Function to print out the sudoku table
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to print
 */
void PrintSudoku(Sudoku *);

//! Function to set a number at a specific location in a sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to fill the number in from 0 - 8
  * @param      unsigned int    The column to fill the number in from 0 - 8
 *  @param      unsigned int    The value to fill into the square from 1 - 9
 *
 *  @returns    boolean         Returns true if successful
 *
 *  Note: This will not do any checks if the number can be placed there, 
 *        it simply changes the value at a location
 */
bool PlaceNumber(Sudoku *, unsigned int, unsigned int, unsigned int);

//! Function to check if a specific cell is free or not
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row of the cell to check
 *  @param      unsigned int    The column of the cell to check
 *
 *  @returns    boolean         Returns true if the cell is empty
 */
bool IsCellEmpty(Sudoku *, unsigned int, unsigned int);

//! Function to determine if a number is able to be placed at a position
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to test the number in
 *  @param      unsigned int    The column to test the number in
 *  @param      unsigned int    The number to test
 *
 *  @returns    boolean         Returns true if succesful
 */
bool CanPlaceNumber(Sudoku *, unsigned int, unsigned int, unsigned int);


//! Function to determine if a number is within a specific row
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to check for the number
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 */
bool IsNumberInRow(Sudoku *, unsigned int, unsigned int);

//! Function to determine if a number is within a specific column
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The column to check for the number
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 */
bool IsNumberInColumn(Sudoku *, unsigned int, unsigned int);

//! Function to determine if a number is within a specific box
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    A row that lies within the box to check
 *  @param      unsigned int    A column that lies within the box to check
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 *
 *  Note: Specify any cell on the grid and it will determine 
 *        if the value is anywhere else in it's respective 3x3 box
 */
bool IsNumberInBox(Sudoku *, unsigned int, unsigned int, unsigned int);

//! Function to determine if a row contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The row to check for completion
 *
 *  @returns    boolean         Returns true if the row is complete
 */
bool IsRowComplete(Sudoku *, unsigned int);

//! Function to determine if a column contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The column to check for completion
 *
 *  @returns    boolean         Returns true if the column is complete
 */
bool IsColumnComplete(Sudoku *, unsigned int);

//! Function to determine if a 3x3 box contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    A column within the box
 *
 *  @returns    boolean         Returns true if the box is complete
 */
bool IsBoxComplete(Sudoku *, unsigned int, unsigned int);

//! Function to determine the specified sudoku contains 1 to 9 in each column, row and box
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to check for completion
 *
 *  @returns    boolean         Returns true if the sudoku is complete
 */
bool IsSudokuComplete(Sudoku *);

//! Function which attempts to solve the 3x3 boxes of the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    unsigned int    The number of cells that were correctly solved
 *
 *  Note: This function uses box checking to determine if there are any places
 *        within boxes that must be a number
 */
unsigned int SolveBoxes(Sudoku *);

//! Function which attempts to solve the rows of the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    unsigned int    The number of cells that were correctly solved
 *
 *  Note: This function uses row checking to determine if there are any places
 *        within the row that must be a number
 */
unsigned int SolveRows(Sudoku *);

//! Function which attempts to solve the columns of the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    unsigned int    The number of cells that were correctly solved
 *
 *  Note: This function uses column checking to determine if there are any places
 *        within the columns that must be a number
 */
unsigned int SolveColumns(Sudoku *);

//! Function which attempts to solve the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    boolean         Returns true if the sudoku was successfully solved
 */
bool SolveSudoku(Sudoku *);


#endif