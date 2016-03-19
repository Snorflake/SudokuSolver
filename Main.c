#include "SudokuSolver.h"

#define MAXBUFFERSIZE 512

#define ISNUMERIC(x) (x > 0x2F && x < 0x3A)

//! This function simply grabs a line of input from the console
/*!
 *    @param        char *            The pointer to the memory to fill with user input
 *    @param        unsigned int    The maximum size in bytes of the input buffer
 *
 *    @author       Daniel Fraser    <danielfraser782@gmail.com>
 */
void __getinput(char *input, unsigned int maxlen)
{
    while(!fgets(input, maxlen, stdin));
}

//! This function splits the first 3 characters of a line into valid xyv format
/*!
 *    @param      char **            A pointer to the pointer containing the input
 *    @param      unsigned int[3]    The xyv structure to fill
 *
 *    @returns    boolean            Whether the first 3 characters were valid xyv numbers
 *
 *    @author     Daniel Fraser      <danielfraser782@gmail.com>
 *
 *    Note: This function increments the ptr as it reads the first 3 characters,
 *          If the first 3 characters are not numeric it returns false.
 */
bool __grabxyv(char **ptr, unsigned int xyv[3])
{
    unsigned int i = 0;

    // break input into x, y, v
    for(i = 0; i < 3; ++i) {
        // make sure each character is from 0-9
        if (ISNUMERIC(**ptr)) {
            // store the numeric value in our array of xyv input
            xyv[i] = (**ptr) - 0x30;

            // subtract 1 from the x and y to support 1 to 9
            if (i < 2) {
                xyv[i] -= 1;
            }

            // iterate the pointer
            (*ptr)++;
        } else {
            // not a number
            return false;
        }
    }

    return true;
}

int main()
{
    char input_buffer[MAXBUFFERSIZE];
    char *ptr;
    unsigned int xyv[3], placed_numbers = 0;

    Sudoku *sudoku = NULL;

    // initialize a new sudoku
    InitializeSudoku(&sudoku);

    printf("_____________________________________________________________________\n"
            "|                    Welcome to sudoku solver v1.0                  |\n"
            "|                                                                   |\n"
            "| Start by entering the numbers in the form xyv                     |\n"
            "| Example: 119 will put a 9 in the top left (x:1, y:1) value: 9     |\n"
            "| You can enter more than one number by separating them with spaces |\n"
            "| Leave the input blank to signal you are finished entering numbers |\n"
            "|___________________________________________________________________|\n");

    // a test sudoku board
#ifdef TEST_SUDOKU
    PlaceNumber(sudoku, 0, 0, 2);
    PlaceNumber(sudoku, 3, 0, 8);
    PlaceNumber(sudoku, 5, 0, 4);
    PlaceNumber(sudoku, 8, 0, 6);
    PlaceNumber(sudoku, 2, 1, 6);
    PlaceNumber(sudoku, 6, 1, 5);
    PlaceNumber(sudoku, 1, 2, 7);
    PlaceNumber(sudoku, 2, 2, 4);
    PlaceNumber(sudoku, 6, 2, 9);
    PlaceNumber(sudoku, 7, 2, 2);
    PlaceNumber(sudoku, 0, 3, 3);
    PlaceNumber(sudoku, 4, 3, 4);
    PlaceNumber(sudoku, 8, 3, 7);
    PlaceNumber(sudoku, 3, 4, 3);
    PlaceNumber(sudoku, 5, 4, 5);
    PlaceNumber(sudoku, 0, 5, 4);
    PlaceNumber(sudoku, 4, 5, 6);
    PlaceNumber(sudoku, 8, 5, 9);
    PlaceNumber(sudoku, 1, 6, 1);
    PlaceNumber(sudoku, 2, 6, 9);
    PlaceNumber(sudoku, 6, 6, 7);
    PlaceNumber(sudoku, 7, 6, 4);
    PlaceNumber(sudoku, 2, 7, 8);
    PlaceNumber(sudoku, 6, 7, 2);
    PlaceNumber(sudoku, 0, 8, 5);
    PlaceNumber(sudoku, 3, 8, 6);
    PlaceNumber(sudoku, 5, 8, 8);
    PlaceNumber(sudoku, 8, 8, 1);
#endif

    // output the board to start
    PrintSudoku(sudoku);

    do {
        // clean our input_buffer each loop
        memset(input_buffer, 0, MAXBUFFERSIZE);
        
        // grab our input
        __getinput(input_buffer, MAXBUFFERSIZE);
        ptr = input_buffer;

        // start at 0 placed numbers this iteration
        placed_numbers = 0;

        // loop till we hit the end of the user input
        while (*ptr != '\0') {

            // break input value into xyv
            if (!__grabxyv(&ptr, xyv)) {
                break;
            }

            // attempt to place the number
            if (CanPlaceNumber(sudoku, xyv[0], xyv[1], xyv[2])) {
                PlaceNumber(sudoku, xyv[0], xyv[1], xyv[2]);
                placed_numbers++;
            }

            // if there is more numbers, scoot forward
            if (*ptr == ' ') {
                ptr++;
            } else { // otherwise we entered something incorrectly
                break;
            }
        }

        // if we placed any numbers
        if (placed_numbers) {
            // print our board after our entry
            PrintSudoku(sudoku);
            printf("Placed %d numbers\n", placed_numbers);
        }

    } while(*input_buffer != '\n');

    // use finished input, solve the sudoku now
    printf("Attempting to solve...\n");
    
    // if we solved the sudoku
    if(SolveSudoku(sudoku)) {
        // print whether we completed successfully or not
        printf("Successfully solved the puzzle\n");
    } else {
        printf("Unable to solve the puzzle\n");
    }

    return 0;
}