/* File contains functions pertaining to the validation of each sub-grid */
#include "msvv.h"
#include "validate.h"
#include <stdio.h>

int validateRow (int rowNum, Buffer1 *buffer1)
{
    int valid = 1;  /* 1 signifies true, 0 false */
    int i, boxVal;
    int validRow[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (i = 0; i < 9; i++)
    {
        boxVal = buffer1->sudokuSolution[rowNum-1][i];
        validRow[boxVal-1]++;
    }

    for (i = 0; i < 9; i++)
    {
        if (validRow[i] == 0)
        {
            valid = 0;
        }
    }
    return valid;
}

int validateAllCols (Buffer1 *buffer1, int* totVal)
{
    int i, valid = 0;
    for (i = 0; i < 9; i++)
    {
        if ((validateCols(i, buffer1)) == 1)
        {
            totVal[i] = 1;
            valid++;
        }
    }

    return valid;
}

int validateCols (int colNum, Buffer1 *buffer1)
{
    int valid = 1; /* 1 Signifies True, 0 for false */
    int validCol[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int i, boxVal;

    for (i = 0; i < 9; i++)
    {
        boxVal = buffer1->sudokuSolution[i][colNum];
        validCol[boxVal-1]++;
    }

    for (i = 0; i < 9; i++)
    {
        if (validCol[i] == 0)
        {
            valid = 0;
        }
    }

    return valid;
}

/*TODO: Divide Sudoku Solution Sub-Grids into 9 1D arrays that contain the
 * results of each sub-grid */
int validateAllGrids (Buffer1 *buffer1)
{
    int totalVal = 0;
    int i, subGrid[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (i = 0; i < 3; i++)
    {
        subGrid[0] = buffer1->sudokuSolution[0][0+i*3];
        subGrid[1] = buffer1->sudokuSolution[0][1+i*3];
        subGrid[2] = buffer1->sudokuSolution[0][2+i*3];
        subGrid[3] = buffer1->sudokuSolution[0][9+i*3];
        subGrid[4] = buffer1->sudokuSolution[0][10+i*3];
        subGrid[5] = buffer1->sudokuSolution[0][11+i*3];
        subGrid[6] = buffer1->sudokuSolution[0][18+i*3];
        subGrid[7] = buffer1->sudokuSolution[0][19+i*3];
        subGrid[8] = buffer1->sudokuSolution[0][20+i*3];

        totalVal += validateSubGrid(subGrid);
    }

    for (i = 0; i < 3; i++)
    {
        subGrid[0] = buffer1->sudokuSolution[0][27+i*3];
        subGrid[1] = buffer1->sudokuSolution[0][28+i*3];
        subGrid[2] = buffer1->sudokuSolution[0][29+i*3];
        subGrid[3] = buffer1->sudokuSolution[0][36+i*3];
        subGrid[4] = buffer1->sudokuSolution[0][37+i*3];
        subGrid[5] = buffer1->sudokuSolution[0][38+i*3];
        subGrid[6] = buffer1->sudokuSolution[0][45+i*3];
        subGrid[7] = buffer1->sudokuSolution[0][46+i*3];
        subGrid[8] = buffer1->sudokuSolution[0][47+i*3];

        totalVal += validateSubGrid(subGrid);
    }

    for (i = 0; i < 3; i++)
    {
        subGrid[0] = buffer1->sudokuSolution[0][54+i*3];
        subGrid[1] = buffer1->sudokuSolution[0][55+i*3];
        subGrid[2] = buffer1->sudokuSolution[0][56+i*3];
        subGrid[3] = buffer1->sudokuSolution[0][63+i*3];
        subGrid[4] = buffer1->sudokuSolution[0][64+i*3];
        subGrid[5] = buffer1->sudokuSolution[0][65+i*3];
        subGrid[6] = buffer1->sudokuSolution[0][72+i*3];
        subGrid[7] = buffer1->sudokuSolution[0][73+i*3];
        subGrid[8] = buffer1->sudokuSolution[0][74+i*3];

        totalVal += validateSubGrid(subGrid);
    }
    return totalVal;
}

int validateSubGrid (int *subGrid)
{
    int valid = 1; /* 1 Signifies True, 0 for false */
    int validGrid[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int i, boxVal;

    for (i = 0; i < 9; i++)
    {
        boxVal = subGrid[i];
        validGrid[boxVal-1]++;
    }

    for (i = 0; i < 9; i++)
    {
        if (validGrid[i] == 0)
        {
            valid = 0;
        }
    }

    return valid;
}
