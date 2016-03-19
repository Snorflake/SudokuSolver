# SudokuSolver
A simple ANSI-C compliant program to solve Sudoku puzzles

Run make test to build TestSudokuSolver which starts you out with a test sudoku puzzle that needs completion.

Run make all to build the SudokuSolver which allows you to enter in the preset numbers of your Sudoku puzzle.

Usage:

Simply enter in each preset number on your Sudoku puzzle separated with spaces in the format xyv.

Example:
123 456 789

The above will enter three numbers into the Sudoku: 3, 6 and 9.

It places the 3 at (1, 2), the 6 at (4, 5), and the 9 at (7, 8)

After you are finished entering all the preset numbers,
simply hit enter with a blank input and the program will attempt to solve the Sudoku.
