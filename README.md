# SudokuSolver
A simple ANSI-C compliant program to solve Sudoku puzzles

# How to Build
Run "make test" to build TestSudokuSolver which starts you out with a test sudoku puzzle that needs completion.

Run make all to build the SudokuSolver which allows you to enter in the preset numbers of your Sudoku puzzle.

# Usage
Simply enter in each preset number on your Sudoku puzzle separated with spaces in the format xyv.

  Example Input: 123 456 789

The above will place a 3 at (1, 2), a 6 at (4, 5), and a 9 at (7, 8)

Once finished entering numbers simply hit enter with a blank input and the program will attempt to solve the Sudoku.
