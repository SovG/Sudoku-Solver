/* File contains functions pertaining to the validation of each sub-grid */
#include "msvv.h"
#include "validate.h"

int validateRow (int rowNum, Buffer1 *buffer1, Buffer2 *buffer2)
{
    int valid = 1;  /* 1 signifies true, 0 false */
    int i, boxVal;
    int validRow[9];

    for (i = 0; i < 9; i++)
    {
        boxVal = buffer1->sudokuSolution[rowNum][i];
        switch (boxVal) {
            case 1:
                validRow[0] = 1;
                break;
            case 2:
                validRow[1] = 2;
                break;
            case 3:
                validRow[2] = 3;
                break;
            case 4:
                validRow[3] = 4;
                break;
            case 5:
                validRow[4] = 5;
                break;
            case 6:
                validRow[5] = 6;
                break;
            case 7:
                validRow[6] = 7;
                break;
            case 8:
                validRow[7] = 8;
                break;
            case 9:
                validRow[8] = 9;
                break;
            }
    }

    for (i = 0; i < 9; i++)
    {
        if (validRow[i] != i+1)
        {
            valid = 0;
        }
    }
    buffer2->validated[rowNum] = valid;
    return valid;
}

int validateAllCols (Buffer1 *buffer1, Buffer2 *buffer2)
{
    int i, totalVal;
    for (i = 0; i < 9; i++)
    {
        if ((validateCols(i, buffer1)) == 1)
        {
            totalVal++;
        }
    }

    buffer2->validated[9] = totalVal;
    return totalVal;
}

int validateCols (int colNum, Buffer1 *buffer1)
{
    int valid = 1; /* 1 Signifies True, 0 for false */
    int validRow[9];
    int i, boxVal;

    for (i = 0; i < 9; i++)
    {
        boxVal = buffer1->sudokuSolution[i][colNum];
        switch (boxVal) {
            case 1:
                validRow[0] = 1;
            case 2:
                validRow[1] = 2;
            case 3:
                validRow[2] = 3;
            case 4:
                validRow[3] = 4;
            case 5:
                validRow[4] = 5;
            case 6:
                validRow[5] = 6;
            case 7:
                validRow[6] = 7;
            case 8:
                validRow[7] = 8;
            case 9:
                validRow[8] = 9;
            }
    }

    for (i = 0; i < 9; i++)
    {
        if (validRow[i] != i+1)
        {
            valid = 0;
        }
    }

    return valid;

}

/*TODO: Divide Sudoku Solution Sub-Grids into 9 1D arrays that contain the
 * results of each sub-grid */
int validateAllGrids (Buffer1 *buffer1, Buffer2 *buffer2)
{
    int totalVal = 0;
    int i;

    for (i = 0; i < 9; i++)
    {
        totalVal += (validateSubGrid(subGrid));
    }
    buffer2->validated[10] = 9;
    return 9;
}

int validateSubGrid (int[] subGrid)
{
    int valid = 1; /* 1 Signifies True, 0 for false */
    int validRow[9];
    int i, boxVal;

    for (i = 0; i < 9; i++)
    {
        boxVal = subGrid[i];
        switch (boxVal) {
            case 1:
                validRow[0] = 1;
            case 2:
                validRow[1] = 2;
            case 3:
                validRow[2] = 3;
            case 4:
                validRow[3] = 4;
            case 5:
                validRow[4] = 5;
            case 6:
                validRow[5] = 6;
            case 7:
                validRow[6] = 7;
            case 8:
                validRow[7] = 8;
            case 9:
                validRow[8] = 9;
            }
    }

    for (i = 0; i < 9; i++)
    {
        if (validRow[i] != i+1)
        {
            valid = 0;
        }
    }

    return valid;

}
