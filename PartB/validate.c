/* File contains functions pertaining to the validation of each sub-grid */
#include "msvv.h"
#include "validate.h"
#include <stdio.h>

int validateRow (int rowNum, SharedMemory* shareMem)
{
    int valid = 1;  /* 1 signifies true, 0 false */
    int i, boxVal;
    int validRow[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (i = 0; i < 9; i++)
    {
        boxVal = shareMem->buffer1[rowNum-1][i];
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

int validateAllCols (SharedMemory* shareMem, int* totalVal)
{
    int i, valid = 0;
    for (i = 0; i < 9; i++)
    {
        if ((validateCols(i, shareMem)) == 1)
        {
            totalVal[i] = 1;
            valid++;
        }
    }

    return valid;
}

int validateCols (int colNum, SharedMemory* shareMem)
{
    int valid = 1; /* 1 Signifies True, 0 for false */
    int validCol[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int i, boxVal;

    for (i = 0; i < 9; i++)
    {
        boxVal = shareMem->buffer1[i][colNum];
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

int validateAllGrids (SharedMemory* shareMem, int* totalVal)
{
    int valid = 0;
    int i, subGrid[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (i = 0; i < 3; i++)
    {
        subGrid[0] = shareMem->buffer1[0][0+i*3];
        subGrid[1] = shareMem->buffer1[0][1+i*3];
        subGrid[2] = shareMem->buffer1[0][2+i*3];
        subGrid[3] = shareMem->buffer1[0][9+i*3];
        subGrid[4] = shareMem->buffer1[0][10+i*3];
        subGrid[5] = shareMem->buffer1[0][11+i*3];
        subGrid[6] = shareMem->buffer1[0][18+i*3];
        subGrid[7] = shareMem->buffer1[0][19+i*3];
        subGrid[8] = shareMem->buffer1[0][20+i*3];

        totalVal[i] = validateSubGrid(subGrid);
        valid += validateSubGrid(subGrid);
    }

    for (i = 0; i < 3; i++)
    {
        subGrid[0] = shareMem->buffer1[0][27+i*3];
        subGrid[1] = shareMem->buffer1[0][28+i*3];
        subGrid[2] = shareMem->buffer1[0][29+i*3];
        subGrid[3] = shareMem->buffer1[0][36+i*3];
        subGrid[4] = shareMem->buffer1[0][37+i*3];
        subGrid[5] = shareMem->buffer1[0][38+i*3];
        subGrid[6] = shareMem->buffer1[0][45+i*3];
        subGrid[7] = shareMem->buffer1[0][46+i*3];
        subGrid[8] = shareMem->buffer1[0][47+i*3];

        totalVal[i+3] = validateSubGrid(subGrid);
        valid += validateSubGrid(subGrid);
    }

    for (i = 0; i < 3; i++)
    {
        subGrid[0] = shareMem->buffer1[0][54+i*3];
        subGrid[1] = shareMem->buffer1[0][55+i*3];
        subGrid[2] = shareMem->buffer1[0][56+i*3];
        subGrid[3] = shareMem->buffer1[0][63+i*3];
        subGrid[4] = shareMem->buffer1[0][64+i*3];
        subGrid[5] = shareMem->buffer1[0][65+i*3];
        subGrid[6] = shareMem->buffer1[0][72+i*3];
        subGrid[7] = shareMem->buffer1[0][73+i*3];
        subGrid[8] = shareMem->buffer1[0][74+i*3];

        totalVal[i+6] = validateSubGrid(subGrid);
        valid += validateSubGrid(subGrid);
    }
    return valid;
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
