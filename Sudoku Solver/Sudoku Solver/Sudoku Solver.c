#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

// .h files
#include "sudoku.h"

int main()
{
    unsigned char** sudo;
    readSudoku(&sudo, "sudoku.in");
    solveSudoku(sudo, "sudoku.out");
    destroySudoku(sudo);
    
    return 0;
}