#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define N 9

#define BLK   "\033[1;30m"
#define RED   "\033[1;31m"
#define GRN   "\033[1;32m"
#define YLW   "\033[1;33m"
#define BLUE  "\033[1;34m"
#define RST   "\033[0m"

int grid[N][N] = {
  {5, 3, 0, 0, 7, 0, 0, 0, 0},
  {6, 0, 0, 1, 9, 5, 0, 0, 0},
  {0, 9, 8, 0, 0, 0, 0, 6, 0},
  {8, 0, 0, 0, 6, 0, 0, 0, 3},
  {4, 0, 0, 8, 0, 3, 0, 0, 1},
  {7, 0, 0, 0, 2, 0, 0, 0, 6},
  {0, 6, 0, 0, 0, 0, 2, 8, 0},
  {0, 0, 0, 4, 1, 9, 0, 0, 5},
  {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

int getFileSize(char *filename) {

  struct stat file_status;
  if (stat(filename, &file_status) < 0)
    return -1;
  else
    return file_status.st_size;

}

void printGrid(void) {
  for (int row = 0; row < N; row++) {
    if (row % 3 == 0)
      printf("+-------+-------+-------+\n");

    for (int col = 0; col < N; col++) {
      if (col % 3 == 0)
        printf("| ");

      if (grid[row][col] == 0)
        printf(". ");
      else if (grid[row][col] > 0)
        printf("%d ", grid[row][col]);
      else
        printf(GRN "%d " RST, abs(grid[row][col]));
    }
    printf("|\n");
  }
  printf("+-------+-------+-------+\n\n");
}

// Function to check if it's safe to place a number in a given position
int isSafe(int row, int col, int num) {
  // Check if the number is not present in the current row and column
  for (int x = 0; x < N; x++)
    if (abs(grid[row][x]) == num || abs(grid[x][col]) == num)
      return 0;

  // Check if the number is not present in the 3x3 subgrid
  int startRow = row - row % 3;
  int startCol = col - col % 3;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (abs(grid[i + startRow][j + startCol]) == num)
        return 0;

  return 1;
}

// Function to solve the Sudoku puzzle using backtracking
int solveSudoku() {
  int row, col;

  // Find an empty position
  int isEmpty = 1;
  for (row = 0; row < N; row++) {
    for (col = 0; col < N; col++) {
      if (grid[row][col] == 0) {
        isEmpty = 0;
        break;
      }
    }
    if (!isEmpty)
      break;
  }

  // If there is no empty position, the puzzle is solved
  if (isEmpty)
    return 1;

  // Try placing numbers 1 to 9 in the current position
  for (int num = 1; num <= N; num++) {
    if (isSafe(row, col, num)) {
      // Place the number if it's safe
      grid[row][col] = num * -1;

      // Recursively solve the rest of the puzzle
      if (solveSudoku())
        return 1;

      // If placing the number doesn't lead to a solution, backtrack
      grid[row][col] = 0;
    }
  }

  // If no number can be placed, backtrack
  return 0;
}

// Function to get one digit within 0-9 from buffer
int getDigit(char ** p) {
  while (1) {
    char a = **p;
    *p = *p + 1;
    if (a == '.')
      return 0;
    else if (a >= '0' && a <= '9')
      return a - '0';
  }
  return -1;
}

// Function to load puzzle from given puzzle file
int loadGrid(char * fileName) {

  char buf[500], *p;
  int row, col;

  printf("the address of buf is %p\n", buf);
  FILE * file = fopen(fileName, "r");
  if (file == NULL) {
    printf("Failed to load puzzle file %s\n", fileName);
    return 1;
  }

  int fileSize = getFileSize(fileName);
  fread(buf, 1, fileSize, file);

  p = &buf[0];
  for (row = 0; row < N; row++)
    for (col = 0; col < N; col++)
      grid[row][col] = getDigit(&p);

  return 0;
}

int main(int argc, char * argv[]) {

  if (argc == 2)
    loadGrid(argv[1]);
  else
    printf("No puzzle provided. Use the default one\n");

  printf("Input:\n");
  printGrid();

  if (solveSudoku()) {
    printf("Output:\n");
    printGrid();
  } else
    printf("No solution exists.");

  return 0;
}
