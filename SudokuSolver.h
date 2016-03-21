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
#define BOX_X(b)  ((b % 3) * 3)
#define BOX_Y(b)  (b - (b % 3))

// simplistic type-unsafe min/max macros
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

// A structure defining an placement entry into the sudoku log
typedef struct {
    // the x position of the placement
    unsigned int x;

    // the y position of the placement
    unsigned int y;

    // the value that was placed
    unsigned int value;
    
    // the probability of this entry
    unsigned int probability;

    // the id of this entry
    unsigned int id;
} LogEntry;

// A structure defining the log of placements
typedef struct {
    // number of entries
    unsigned int count;

    // the list of entries
    LogEntry *entries;
} Log;

// A structure defining what a guess on a sudoku board looks like
typedef struct {
    // the x postion of the guess
    unsigned int x;

    // the y position of the guess
    unsigned int y;

    // the value of the guess
    unsigned int value;

    // the probability this guess is correct
    unsigned int probability;
} Guess;

// A structure defining a list of guesses
typedef struct {
    // the number of guesses in the list
    unsigned int count;

    // the list of pointers to guesses
    Guess **guesses;
} GuessList;

// A structure defining a sudoku that needs solving
typedef struct {
    // the grid itself
    unsigned int grid[9][9];

    // the minimum guess threshold %
    unsigned int threshold;

    // the maximum number of consecutive guesses
    unsigned int maxguesscount;

    // the list of guesses that have been made
    GuessList *guesslist;
} Sudoku;

//! Function to create an initialize a new guess
/*!
 *  @param      unsigned int    The x position of the new guess
 *  @param      unsigned int    The y position of the new guess
 *  @param      unsigned int    The value of the new guess
 *  @param      unsigned int    The probability of the new guess
 *
 *  @returns    Guess *         A pointer to the new guess object or NULL
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
Guess *CreateGuess(unsigned int X, unsigned int Y, unsigned int Value, unsigned int Probability);

//! Function to destroy an initialized new guess
/*!
 *  @param      Guess *         A pointer to the guess object to destroy
 *
 *  @returns    boolean         Whether the guess object was successfully destroyed
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool DestroyGuess(Guess *guess);

//! Function to initialize a new guess list
/*!
 *  @param      Guesslist **    A pointer to a pointer that will receive the initialized guess list
 *
 *  @returns    boolean         Whether the guess list was initialized
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool InitializeGuessList(GuessList **list);

//! Function to safely cleanup all guesses within a guess list without destroying the list
/*!
 *  @param      GuessList *     A pointer ot the guess list to clean up
 *
 *  @returns    boolean         Whether the guess list was cleaned up successfully
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool EmptyGuessList(GuessList *list);

//! Function to cleanup a guess list and all containing guesses
/*!
 *  @param      Guesslist *     A pointer to the initialized guess list
 *
 *  @returns    boolean         Whether the guess list was erased
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool DestroyGuessList(GuessList *list);

//! Function to create and append a guess to the guesslist
/*!
 *  @param      Guesslist *     The guess list to append the guess to
 *  @param      unsigned int    The x position of the new guess
 *  @param      unsigned int    The y position of the new guess
 *  @param      unsigned int    The value of the new guess
 *  @param      unsigned int    The probability of the new guess
 *
 *  @returns    boolean         Whether the guess was successfully appended
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool AppendGuess(GuessList *list, unsigned int X, unsigned int Y, unsigned int Value, unsigned int Probability);

//! Function to remove a guess from the end of the guesslist
/*!
 *  @param      Guesslist *     The guess list to remove the guess from
 *
 *  @returns    boolean         Whether the guess was successfully removed
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool RemoveGuess(GuessList *guesslist);

//! Function to initialize a new sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to initialize
 *  @param      float           The probabilty threshold required for a guess to be made
 *  @param      unsigned int    The maxmimum number of consecutive guesses
 *
 *  @returns    boolean         Returns true if successful
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool InitializeSudoku(Sudoku **sudoku, unsigned int guessthreshold, unsigned int maxguesses);

//! Function to safely cleanup a sudoku object
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to initialize
 *
 *  @returns    boolean         Returns true if sudoku cleaned up successfully
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool DestroySudoku(Sudoku *sudoku);

//! Function to print out the sudoku table
/*!
 *  @param    Sudoku*         A pointer to the sudoku object to print
 *
 *  @author   Daniel Fraser   <danielfraser782@gmail.com>
 */
void PrintSudoku(Sudoku *sudoku);

//! Function to place a number at a specific location in a sudoku and record it into the log
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The x position of the cell to fill the number in from 0 - 8
 *  @param      unsigned int    The y position of the cell to fill the number in from 0 - 8
 *  @param      unsigned int    The value to fill into the square from 1 - 9
 *
 *  @returns    boolean         Returns true if successful
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This will not do any checks if the number can be placed there, 
 *        it simply changes the value at a location
 */
bool PlaceNumber(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value);

//! Function to check if a specific cell is free or not
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The x position of the cell to check
 *  @param      unsigned int    The y position of the cell to check
 *
 *  @returns    boolean         Returns true if the cell is empty
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsCellEmpty(Sudoku *sudoku, unsigned int X, unsigned int Y);

//! Function to determine if a number is able to be placed at a position
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The x position to test the number in
 *  @param      unsigned int    The y position to test the number in
 *  @param      unsigned int    The number to test
 *
 *  @returns    boolean         Returns true if succesful
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool CanPlaceNumber(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value);

//! Function to determine if a number is within a specific row
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The Y value where all horizontal cells are checked
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsNumberInRow(Sudoku *sudoku, unsigned int Y, unsigned int value);

//! Function to determine if a number is within a specific column
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The X value where all vertical cells are checked
 *  @param      unsigned int    The number to find
 *
 *  @returns    boolean         Returns true if the number is found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsNumberInColumn(Sudoku *sudoku, unsigned int X, unsigned int value);

//! Function to determine if a number is within a specific box
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The X value of a cell within the box to check
 *  @param      unsigned int    The Y value of a cell within the box to check
 *  @param      unsigned int    The number to find in the box
 *
 *  @returns    boolean         Returns true if the number is found within the 3x3 box
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: Specify any cell and it will determine if the value is anywhere else in the 3x3 box
 */
bool IsNumberInBox(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value);

//! Function to determine if a row contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The Y value where all horizontal cells are checked
 *
 *  @returns    boolean         Returns true if the row is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsRowComplete(Sudoku *sudoku, unsigned int Y);

//! Function to determine if a column contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The X value where all vertical values are checked
 *
 *  @returns    boolean         Returns true if the column is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsColumnComplete(Sudoku *sudoku, unsigned int X);

//! Function to determine if a 3x3 box contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The x value of a cell within the 3x3 box
 *  @param      unsigned int    The y value of a cell within the 3x3 box
 *
 *  @returns    boolean         Returns true if the box contains all numbers from 1-9
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsBoxComplete(Sudoku *sudoku, unsigned int X, unsigned int Y);

//! Function to determine the specified sudoku contains 1 to 9 in each column, row and box
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to check for completion
 *
 *  @returns    boolean         Returns true if the sudoku is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsSudokuComplete(Sudoku *sudoku);

//! Function which determines the probability that a number is correct based on the 3x3 box
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *  @param      unsigned int    The x position of a cell within the box to test
 *  @param      unsigned int    The y position of a cell within the box to test
 *  @param      unsigned int    The value to test in the cell
 *
 *  @returns    unsigned int    The probability from 0 to 100 that the given number is correct
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This function uses 3x3 box checking to determine the probability of a correct placement
 */
unsigned int BoxProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value);

//! Function which determines the probability that a number is correct based on the 1x9 row
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *  @param      unsigned int    The x position of a cell within the row to test
 *  @param      unsigned int    The y position of a cell within the row to test
 *  @param      unsigned int    The value to test in the cell
 *
 *  @returns    unsigned int    The probability from 0 to 100 that the given number is correct
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This function uses 1x9 row checking to determine the probability of a correct placement
 */
unsigned int RowProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value);

//! Function which determines the probability that a number is correct based on the 1x9 column
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *  @param      unsigned int    The x position of a cell within the column to test
 *  @param      unsigned int    The y position of a cell within the column to test
 *  @param      unsigned int    The value to test in the cell
 *
 *  @returns    unsigned int    The probability from 0 to 100 that the given number is correct
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This function uses 1x9 column checking to determine the probability of a correct placement
 */
unsigned int ColumnProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value);

//! Function which determines the max probability of a correct guess in a cell
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *  @param      unsigned int    The x position of the cell to test for probability
 *  @param      unsigned int    The y position of the cell to test for probability
 *  @param      unsigned int    The value to test for probability
 *
 *  @returns    unsigned int    The highest probability that the value is correct
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 *
 *  Note: This function uses box, row and column checking to determine the max probability
 */
unsigned int MaxProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value);

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
unsigned int SolveBoxes(Sudoku *sudoku);

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
unsigned int SolveRows(Sudoku *sudoku);

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
unsigned int SolveColumns(Sudoku *sudoku);

//! Function which attempts to find the best guesses within a box with a probability above threshold
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to search
 *  @param      unsigned int    The minimum threshold to pass for a guess (0 to 100)
 *  @param      unsigned int    The x position of a cell within the box to check
 *  @param      unsigned int    The y position of a cell within the box to check
 *  @param      SudokuGuess *    A pointer to a guess list that will receive all the best guesses
 *
 *  @returns    boolean         Returns true if a possible guess was found, false if nothing was found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool FindBestGuessesBox(Sudoku *sudoku, unsigned int threshold, unsigned int X, unsigned int Y, GuessList *list);

//! Function which attempts to find the best guesses within a row with a probability above threshold
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to search
 *  @param      unsigned int    The minimum threshold to pass for a guess (0 to 100)
 *  @param      unsigned int    The y position of the row to check
 *  @param      SudokuGuess *    A pointer to a guess list that will receive all the best guesses
 *
 *  @returns    boolean         Returns true if a possible guess was found, false if nothing was found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool FindBestGuessesRow(Sudoku *sudoku, unsigned int threshold, unsigned int Y, GuessList *list);

//! Function which attempts to find the best guesses within a column with a probability above threshold
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to search
 *  @param      unsigned int    The minimum threshold to pass for a guess (0 to 100)
 *  @param      unsigned int    The x position of the column to check
 *  @param      SudokuGuess *    A pointer to a guess list that will receive all the best guesses
 *
 *  @returns    boolean         Returns true if a possible guess was found, false if nothing was found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool FindBestGuessesColumn(Sudoku *sudoku, unsigned int threshold, unsigned int X, GuessList *list);

//! Function which attempts to find all the best guesses within the entire sudoku with a probability above threshold
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to search
 *  @param      unsigned int    The minimum threshold to pass for a guess (0 to 100)
 *  @param      SudokuGuess *    A pointer to a guess list that will receive all the best guesses
 *
 *  @returns    boolean         Returns true if a possible guess was found, false if nothing was found
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool FindBestGuesses(Sudoku *sudoku, unsigned int threshold, GuessList *list);

//! Function which attempts to solve the sudoku
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to attempt to solve
 *
 *  @returns    boolean         Returns true if the sudoku was successfully solved
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool SolveSudoku(Sudoku *sudoku);

#endif