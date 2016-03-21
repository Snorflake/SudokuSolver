#include "SudokuSolver.h"

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
Guess *CreateGuess(unsigned int X, unsigned int Y, unsigned int Value, unsigned int Probability)
{
    Guess *new_guess = NULL;

    // sanity
    if (X > 8
        || Y > 8
        || Value > 9
        || Value == 0) {
        return NULL;
    }

    // ensure probability is a valid number
    if (Probability > 100) {
        Probability = 100;
    }

    // allocate our new guess
    new_guess = (Guess*)calloc(1, sizeof(Guess));

    // sanity our new guess
    if (!new_guess) {
        return NULL;
    }

    // initialize our new guess
    new_guess->x = X;
    new_guess->y = Y;
    new_guess->value = Value;
    new_guess->probability = Probability;

    // success
    return new_guess;
}

//! Function to destroy an initialized new guess
/*!
 *  @param      Guess *         A pointer to the guess object to destroy
 *
 *  @returns    boolean         Whether the guess object was successfully destroyed
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool DestroyGuess(Guess *guess)
{
    // sanity
    if (!guess) {
        return false;
    }

    // free it
    free(guess);

    return true;
}

//! Function to initialize a new guess list
/*!
 *  @param      Guesslist **    A pointer to a pointer that will receive the initialized guess list
 *
 *  @returns    boolean         Whether the guess list was initialized
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool InitializeGuessList(GuessList **list)
{
    GuessList *new_list = NULL;

    // sanity
    if (!list) {
        return false;
    }

    // allocate memory for our guess list
    new_list = (GuessList*)calloc(1, sizeof(GuessList));

    // sanity check our guess list
    if (!new_list) {
        return false;
    }

    // zero out our new guess list
    memset(new_list, 0, sizeof(GuessList));

    // success
    return true;
}

//! Function to safely cleanup all guesses within a guess list without destroying the list
/*!
 *  @param      GuessList *     A pointer ot the guess list to clean up
 *
 *  @returns    boolean         Whether the guess list was cleaned up successfully
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool EmptyGuessList(GuessList *list)
{
    unsigned int i = 0;

    // sanity
    if (!list) {
        return false;
    }

    // cleanup guesses
    if (list->guesses 
        && list->count) {
        // iterate from end to start removing each guess
        for (i = list->count; i > 0; --i) {
            if (!RemoveGuess(list)) {
                return false;
            }
        }
    }

    return true;
}

//! Function to safely cleanup a guess list and all containing guesses
/*!
 *  @param      Guesslist *     A pointer to the initialized guess list
 *
 *  @returns    boolean         Whether the guess list was erased
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool DestroyGuessList(GuessList *list)
{
    // sanity
    if (!list) {
        return false;
    }

    // cleanup all the guesses
    if (!EmptyGuessList(list)) {
        return false;
    }

    // cleanup the list
    free(list);

    // success
    return true;
}

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
bool AppendGuess(GuessList *list, unsigned int X, unsigned int Y, unsigned int Value, unsigned int Probability)
{
    unsigned int guesscount = 0;
    Guess *new_guess = NULL;

    // sanity
    if (!list
        || X > 8
        || Y > 8
        || Value > 9
        || Value == 0) {
        return NULL;
    }

    // try to create our new guess
    new_guess = CreateGuess(X, Y, Value, Probability);

    // sanity check it
    if (!new_guess) {
        return false;
    }

    // grab the number of guesses in the list and add one
    guesscount = list->count + 1;

    // realloc the array of guesses
    list->guesses = (Guess**)realloc(list->guesses, guesscount * sizeof(Guess*));

    // sanity the realloc
    if (!list->guesses) {
        return false;
    }

    // insert our new guess
    list->guesses[list->count] = new_guess;

    // adjust the count
    list->count = guesscount;

    // success
    return true;
}

//! Function to remove and cleanup a guess from the end of the guesslist
/*!
 *  @param      Guesslist *     The guess list to remove the guess from
 *
 *  @returns    boolean         Whether the guess was successfully removed
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool RemoveGuess(GuessList *list)
{
    unsigned int guesscount = 0;

    // sanity
    if (!list
        || list->count < 1) {
        return false;
    }

    // grab the number of guesses in the list and remove one
    guesscount = list->count - 1;

    // try to destroy the last guess
    if (!DestroyGuess(list->guesses[guesscount])) {
        return false;
    }

    // realloc the array of guesses
    list->guesses = (Guess**)realloc(list->guesses, guesscount * sizeof(Guess*));

    // sanity the realloc
    if (!list->guesses) {
        return false;
    }

    // adjust the count
    list->count = guesscount;

    // success
    return true;
}

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
bool InitializeSudoku(Sudoku **sudoku, unsigned int guessthreshold, unsigned int maxguesses)
{
    // sanity check
    if (!sudoku) {
        return false;
    }

    // allocate a new sudoku
    Sudoku *new_sudoku = (Sudoku*)calloc(1, sizeof(Sudoku));

    // sanity check our new sudoku
    if (!new_sudoku) {
        return false;
    }

    // zero out the grid
    memset(new_sudoku->grid, 0, sizeof(new_sudoku->grid));

    // assign the guess threshold
    new_sudoku->threshold = guessthreshold;

    // assign the max guess count
    new_sudoku->maxguesscount = maxguesses;

    // if we're allowed to guess at all
    if (maxguesses) {
        
    }

    // assign the sudoku
    *sudoku = new_sudoku;

    // success
    return true;
}

//! Function to safely cleanup a sudoku object
/*!
 *  @param      Sudoku*         A pointer to the sudoku object to initialize
 *
 *  @returns    boolean         Returns true if sudoku cleaned up successfully
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool DestroySudoku(Sudoku *sudoku)
{
    // sanity
    if (!sudoku) {
        return false;
    }

    // if we have a list of guesses
    if (sudoku->guesslist) {
        // free the guess list
        free(sudoku->guesslist);
    }
        
    // erase our object
    free(sudoku);

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
bool PlaceNumber(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value)
{
    // sanity check
    if (!sudoku 
        || Y > 8
        || X > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // assign the value
    sudoku->grid[Y][X] = value;

    // success
    return true;
}

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
bool IsCellEmpty(Sudoku *sudoku, unsigned int X, unsigned int Y)
{
    // sanity
    if (!sudoku) {
        return false;
    }

    // is this cell a 0?
    return (sudoku->grid[Y][X] == 0);
}

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
bool CanPlaceNumber(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value)
{
    // sanity check
    if (!sudoku 
        || Y > 8
        || X > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // make sure the cell is empty
    if (!IsCellEmpty(sudoku, X, Y)) {
        return false;
    }

    // check row
    if (IsNumberInRow(sudoku, Y, value)) {
        return false;
    }

    // check column
    if (IsNumberInColumn(sudoku, X, value)) {
        return false;
    }
    
    // check box
    if (IsNumberInBox(sudoku, X, Y, value)) {
        return false;
    }

    // we can place a number here
    return true;
}

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
bool IsNumberInRow(Sudoku *sudoku, unsigned int Y, unsigned int value)
{
    unsigned int i = 0;

    // sanity check
    if (!sudoku 
        || Y > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // iterate this row
    for (i = 0; i < 9; ++i) {
        if (sudoku->grid[Y][i] == value) {
            return true;
        }
    }

    return false;
}

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
bool IsNumberInColumn(Sudoku *sudoku, unsigned int X, unsigned int value)
{
    unsigned int i = 0;

    // sanity check
    if (!sudoku 
        || X > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // iterate this column
    for (i = 0; i < 9; ++i) {
        if (sudoku->grid[i][X] == value) {
            return true;
        }
    }

    return false;
}

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
bool IsNumberInBox(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value)
{
    // the top left cell of this box
    unsigned int boxX, boxY = 0, i = 0, j = 0;

    // sanity check
    if (!sudoku 
        || Y > 8
        || X > 8
        || value > 9
        || value == 0) {
            return false;
    }

    // grab the top left cell of this box
    boxX = X - (X % 3);
    boxY = Y - (Y % 3);

    // check this box
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (sudoku->grid[boxY + i][boxX + j] == value) {
                return true;
            }
        }
    }

    return false;
}

//! Function to determine if a row contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The Y value where all horizontal cells are checked
 *
 *  @returns    boolean         Returns true if the row is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsRowComplete(Sudoku *sudoku, unsigned int Y)
{
    unsigned int i = 0;
    
    // sanity check
    if (!sudoku 
        || Y > 8) {
            return false;
    }

    // find each number in this row
    for (i = 1; i < 10; ++i) {
        if (!IsNumberInRow(sudoku, Y, i)) {
            return false;
        }
    }

    return true;
}

//! Function to determine if a column contains all numbers from 1 to 9
/*!
 *  @param      Sudoku*         A pointer to the sudoku object
 *  @param      unsigned int    The X value where all vertical values are checked
 *
 *  @returns    boolean         Returns true if the column is complete
 *
 *  @author     Daniel Fraser   <danielfraser782@gmail.com>
 */
bool IsColumnComplete(Sudoku *sudoku, unsigned int X)
{
    unsigned int i = 0;
    
    // sanity check
    if (!sudoku 
        || X > 8) {
            return false;
    }

    // find each number in this column
    for (i = 1; i < 10; ++i) {
        if (!IsNumberInColumn(sudoku, X, i)) {
            return false;
        }
    }

    return true;
}

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
bool IsBoxComplete(Sudoku *sudoku, unsigned int X, unsigned int Y)
{
    unsigned int i = 0;

    // sanity check
    if (!sudoku 
        || X > 8
        || Y > 8) {
            return false;
    }
    
    // find each number in this box
    for (i = 1; i < 10; ++i) {
        if (!IsNumberInBox(sudoku, X, Y, i)) {
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
        if (!IsBoxComplete(sudoku, BOX_X(i), BOX_Y(i))) {
            return false;
        }
    }

    return true;
}

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
unsigned int BoxProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value)
{
    unsigned int x = 0, y = 0, places = 0, box_x_start = 0, box_x_end = 0, box_y_start = 0, box_y_end = 0;

    // sanity check
    if (!sudoku 
        || Y > 8
        || X > 8
        || value > 9
        || value == 0) {
            return 0;
    }

    // the amount of places within this box we can place the number
    places = 1;
    
    box_x_start = (X - (X % 3));
    box_x_end = ((X - (X % 3)) + 3);
    box_y_start = (Y - (Y % 3));
    box_y_end = ((Y - (Y % 3)) + 3);

    // iterate each cell within this box vertically
    for (y = box_y_start; y < box_y_end; ++y) {
        // iterate each cell within this box horizontally
        for (x = box_x_start; x < box_x_end; ++x) {
            // if this isn't the cell we're originally trying to place a number in
            if (x != X || y != Y) {
                // can we place a number in this cell
                if (CanPlaceNumber(sudoku, x, y, value)) {
                    // we can place the number in this other box
                    places++;
                }
            }
        }
    }

    return 100 / places;
}

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
unsigned int RowProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value)
{
    unsigned int x = 0, places = 0;

    // sanity check
    if (!sudoku 
        || Y > 8
        || X > 8
        || value > 9
        || value == 0) {
            return 0;
    }

    // the amount of places within this box we can place the number
    places = 1;

    // iterate each cell within this row horizontally
    for (x = 0; x < 9; ++x) {
        // if this isn't the cell we're originally trying to place a number in
        if (x != X) {
            // can we place a number in this cell
            if (CanPlaceNumber(sudoku, x, Y, value)) {
                // we can place the number in this other box
                places++;
            }
        }
    }

    return 100 / places;
}

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
unsigned int ColumnProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value)
{
    unsigned int y = 0, places = 0;

    // sanity check
    if (!sudoku 
        || Y > 8
        || X > 8
        || value > 9
        || value == 0) {
            return 0;
    }

    // the amount of places within this box we can place the number
    places = 1;

    // iterate each cell within this row horizontally
    for (y = 0; y < 9; ++y) {
        // if this isn't the cell we're originally trying to place a number in
        if (y != Y) {
            // can we place a number in this cell
            if (CanPlaceNumber(sudoku, X, y, value)) {
                // we can place the number in this other box
                places++;
            }
        }
    }

    return 100 / places;
}

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
unsigned int MaxProbability(Sudoku *sudoku, unsigned int X, unsigned int Y, unsigned int value)
{
    int max_probability = 0, box_probability = 0, row_probability = 0, col_probability = 0;

    // sanity check
    if (!sudoku 
        || Y > 8
        || X > 8
        || value > 9
        || value == 0) {
            return 0;
    }

    // grab the box, row and column probabilities
    box_probability = BoxProbability(sudoku, X, Y, value);
    row_probability = RowProbability(sudoku, X, Y, value);
    col_probability = ColumnProbability(sudoku, X, Y, value);

    // determine the max of the 3 probabilities
    max_probability = MAX(MAX(box_probability, row_probability), col_probability);

    // return the maximum of the 3
    return max_probability;
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
    unsigned int b = 0, y = 0, x = 0, v = 0;
    unsigned int solvednumbers = 0;

    // sanity
    if (!sudoku) {
        return 0;
    }

    // iterate each box
    for (b = 0; b < 9; ++b) {
        // make sure this box isn't completed yet
        if (!IsBoxComplete(sudoku, BOX_X(b), BOX_Y(b))) {
            // iterate vertically
            for (y = BOX_Y(b); y < (BOX_Y(b) + 3); ++y) {
                // iterate horizontally
                for (x = BOX_X(b); x < (BOX_X(b) + 3); ++x) {
                    // if this cell is empty
                    if (IsCellEmpty(sudoku, x, y)) {
                        // iterate values
                        for (v = 1; v < 10; ++v) {
                            // if we can place a number here, check every other cell in this box
                            if (CanPlaceNumber(sudoku, x, y, v)) {
                                // if we didn't discover any other places in this box that the number can be placed
                                if (BoxProbability(sudoku, x, y, v) == 100) {
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
    unsigned int y = 0, x = 0, v = 0;
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
                        // if we can place a number here, check every other cell in this row
                        if (CanPlaceNumber(sudoku, x, y, v)) {
                            // if we didn't discover any other places in this row that the number can be placed
                            if (RowProbability(sudoku, x, y, v) == 100) {
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
    unsigned int y = 0, x = 0, v = 0;
    unsigned int solvednumbers = 0;

    // sanity
    if (!sudoku) {
        return 0;
    }

    // iterate each column
    for (x = 0; x < 9; ++x) {
        // make sure this column isn't completed yet
        if (!IsColumnComplete(sudoku, x)) {
            // iterate vertically
            for (y = 0; y < 9; ++y) {
                // if this cell is empty
                if (IsCellEmpty(sudoku, x, y)) {
                    // iterate values
                    for (v = 1; v < 10; ++v) {
                        // if we can place a number here, check every other cell in this column
                        if (CanPlaceNumber(sudoku, x, y, v)) {
                            // if we didn't discover any other places in this column that the number can be placed
                            if (ColumnProbability(sudoku, x, y, v) == 100) {
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
bool FindBestGuessesBox(Sudoku *sudoku, unsigned int threshold, unsigned int X, unsigned int Y, GuessList *list)
{
    unsigned int x = 0, y = 0, v = 0, box_x_start = 0, box_x_end = 0, box_y_start = 0, box_y_end = 0, max_probability = 0, probability = 0;
    bool complete_iteration = false;

    // sanity check
    if (!sudoku
        || !list
        || X > 8
        || Y > 8) {
            return false;
    }

    // ensure threshold is within 100, not really necessary
    if (threshold > 100) {
        threshold = 100;
    }

    // grab the bounds of this 3x3 box
    box_x_start = (X - (X % 3));
    box_x_end = ((X - (X % 3)) + 3);
    box_y_start = (Y - (Y % 3));
    box_y_end = ((Y - (Y % 3)) + 3);

    // This label is used to iterate all cells twice, first to find the highest probability
    // the second time to store all guesses with the highest probability
REITERATE:

    // iterate each cell within this box vertically
    for (y = box_y_start; y < box_y_end; ++y) {
        // iterate each cell within this box horizontally
        for (x = box_x_start; x < box_x_end; ++x) {
            // if this cell is empty
            if (IsCellEmpty(sudoku, x, y)) {
                // iterate all values
                for (v = 0; v < 9; ++v) {
                    // grab the probability of this cell
                    probability = BoxProbability(sudoku, x, y, v);

                    // ensure our probability meets our threshold
                    if (probability >= threshold) {
                        // if this is the second iteration
                        if (complete_iteration) {
                            // if this probability is = to the max
                            if (probability == max_probability) {
                                // attempt to append this new guess to the list
                                if (!AppendGuess(list, x, y, v, probability)) {
                                    return false;
                                }
                            }
                        } else { // if this is the first iteration
                            // determine the maxmimum probability out of all cells
                            if (probability > max_probability) {
                                max_probability = probability;
                            }
                        }
                    }
                }
            }
        }
    }

    // if we completed a full iteration to determine the highest probability
    if (!complete_iteration) {
        // go back and iterate again this time storing all guesses with probability == to the highest
        complete_iteration = true;
        goto REITERATE;
    }

    // success
    return (list->count > 0);
}

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
bool FindBestGuessesRow(Sudoku *sudoku, unsigned int threshold, unsigned int Y, GuessList *list)
{
    unsigned int x = 0, v = 0, max_probability = 0, probability = 0;
    bool complete_iteration = false;

    // sanity check
    if (!sudoku
        || !list
        || Y > 8) {
            return false;
    }

    // ensure threshold is within 100, not really necessary
    if (threshold > 100) {
        threshold = 100;
    }

    // This label is used to iterate all cells twice, first to find the highest probability
    // the second time to store all guesses with the highest probability
REITERATE:


    // iterate each cell within this row horizontally
    for (x = 0; x < 9; ++x) {
        // if this cell is empty
        if (IsCellEmpty(sudoku, x, Y)) {
            // iterate all values
            for (v = 0; v < 9; ++v) {
                // grab the probability of this cell
                probability = RowProbability(sudoku, x, Y, v);

                // ensure our probability meets our threshold
                if (probability >= threshold) {
                    // if this is the second iteration
                    if (complete_iteration) {
                        // if this probability is = to the max
                        if (probability == max_probability) {
                            // attempt to append this new guess to the list
                            if (!AppendGuess(list, x, Y, v, probability)) {
                                return false;
                            }
                        }
                    } else { // if this is the first iteration
                        // determine the maxmimum probability out of all cells
                        if (probability > max_probability) {
                            max_probability = probability;
                        }
                    }
                }
            }
        }
    }
    

    // if we completed a full iteration to determine the highest probability
    if (!complete_iteration) {
        // go back and iterate again this time storing all guesses with probability == to the highest
        complete_iteration = true;
        goto REITERATE;
    }

    // success
    return (list->count > 0);
}

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
bool FindBestGuessesColumn(Sudoku *sudoku, unsigned int threshold, unsigned int X, GuessList *list)
{
    unsigned int y = 0, v = 0, max_probability = 0, probability = 0;
    bool complete_iteration = false;

    // sanity check
    if (!sudoku
        || !list
        || X > 8) {
            return false;
    }

    // ensure threshold is within 100, not really necessary
    if (threshold > 100) {
        threshold = 100;
    }

    // This label is used to iterate all cells twice, first to find the highest probability
    // the second time to store all guesses with the highest probability
REITERATE:


    // iterate each cell within this column vertically
    for (y = 0; y < 9; ++y) {
        // if this cell is empty
        if (IsCellEmpty(sudoku, X, y)) {
            // iterate all values
            for (v = 0; v < 9; ++v) {
                // grab the probability of this cell
                probability = RowProbability(sudoku, X, y, v);

                // ensure our probability meets our threshold
                if (probability >= threshold) {
                    // if this is the second iteration
                    if (complete_iteration) {
                        // if this probability is = to the max
                        if (probability == max_probability) {
                            // attempt to append this new guess to the list
                            if (!AppendGuess(list, X, y, v, probability)) {
                                return false;
                            }
                        }
                    } else { // if this is the first iteration
                        // determine the maxmimum probability out of all cells
                        if (probability > max_probability) {
                            max_probability = probability;
                        }
                    }
                }
            }
        }
    }
    

    // if we completed a full iteration to determine the highest probability
    if (!complete_iteration) {
        // go back and iterate again this time storing all guesses with probability == to the highest
        complete_iteration = true;
        goto REITERATE;
    }

    // success
    return (list->count > 0);
}

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
bool FindBestGuesses(Sudoku *sudoku, unsigned int threshold, GuessList *list)
{
    unsigned int z = 0, g = 0, max_probability = 0, probability = 0;
    bool complete_iteration = false;
    GuessList *guesses_list = NULL;

    // sanity check
    if (!sudoku
        || !list) {
            return false;
    }

    // ensure threshold is within 100, not really necessary
    if (threshold > 100) {
        threshold = 100;
    }

    // initialize our guess list
    if (!InitializeGuessList(&guesses_list)) {
        return false;
    }

    // iterate all boxes, rows and columns and find guesses above the threshold
    for (z = 0; z < 9; ++z) {
        // find the best guesses for this box
        FindBestGuessesBox(sudoku, threshold, BOX_X(z), BOX_Y(z), guesses_list);

        // find the best guesses for this row
        FindBestGuessesRow(sudoku, threshold, z, guesses_list);

        // find the best guesses for this column
        FindBestGuessesColumn(sudoku, threshold, z, guesses_list);
    }

    // used to reiterate after the highest guess probability has been found
    // once the highest is found all the guesses with the highest probability can be added
REITERATE:
    for (g = 0; g < guesses_list->count; ++g) {
        // the probability for this guess
        probability = guesses_list->guesses[g]->probability;
        
        // if this is the second iteration
        if (complete_iteration) {
            // if this guess has the highest probability
            if (probability == max_probability) {
                // Append a copy of this guess to our output list
                if (!AppendGuess(list, 
                    guesses_list->guesses[g]->x,
                    guesses_list->guesses[g]->y,
                    guesses_list->guesses[g]->value,
                    guesses_list->guesses[g]->probability)) {
                    // make sure our guess appending worked
                    return false;
                }
            }
        } else { // if this is the first iteration
            // store the highest probability
            if (probability > max_probability) {
                max_probability = probability;
            }
        }
    }

    // once we've gone through the entire list to determine the highest probability
    if (!complete_iteration) {
        // go back through the list and save the guesses with the highest probability
        complete_iteration = true;
        goto REITERATE;
    }
    
    // cleanup the guess list
    DestroyGuessList(guesses_list);

    // return whether any were found
    return (list->count > 0);
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
    bool progress = true, complete = false;
#ifdef CANGUESS
    bool can_guess = false;
    unsigned int guess_count = 0;
    GuessList *guess_list = NULL;
    Guess *last_guess = NULL;
#endif

    // sanity
    if (!sudoku) {
        return false;
    }

#ifdef CANGUESS
    // make a new guess list for guessing purposes
    if (!InitializeGuessList(&guess_list)) {
        return false;
    }
#endif
    
    // loop till we can't make any more progress or the sudoku is solved
    while (progress && !complete) {
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

        // check for completion
        complete = IsSudokuComplete(sudoku);

#ifdef CANGUESS
        // if we can't make progress, and the sudoku isn't complete, we need to guess
        if (!progress && !complete) {
           // see if we can guess
           can_guess = FindBestGuesses(sudoku, sudoku->threshold, guess_list);

           // if we're able to make a guess
           if (can_guess) {

               if(guess_count < sudoku->maxguesscount) {
               
               // make sure we can place this guess
               if (CanPlaceNumber(sudoku, 
                   guess_list->guesses[guess_count]->x,
                   guess_list->guesses[guess_count]->y,
                   guess_list->guesses[guess_count]->value)) {
                   // place the guess
                   PlaceNumber(sudoku, 
                       guess_list->guesses[guess_count]->x,
                       guess_list->guesses[guess_count]->y,
                       guess_list->guesses[guess_count]->value);
                       
                   // set the last guess
                   last_guess = guess_list->guesses[guess_count];

                   // increment our guesses
                   guess_count++;
               }
           }

           // if we were able to guess, we made progress and should re-try
           progress = can_guess;
        }
#endif
    }

#ifdef CANGUESS
    // done with our guess list
    DestroyGuessList(guess_list);
#endif

    // print our sudoku after all attempts to solve have been made
    PrintSudoku(sudoku);
    
    // return whether we completed or not
    return IsSudokuComplete(sudoku);
}
