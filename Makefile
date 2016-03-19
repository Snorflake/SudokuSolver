all:
	gcc -Wall Main.c SudokuSolver.c -o SudokuSolver
	
test:
	gcc	-Wall -g -DTEST_SUDOKU Main.c SudokuSolver.c -o TestSudokuSolver