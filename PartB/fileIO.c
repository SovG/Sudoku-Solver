/* File contains functions pertaining to File IO for msvv.c */
#include "msvv.h"
#include <stdio.h>

/* Function reads the Sudoku Solution from file and loads it into Buffer 1 */
int readFile (SharedMemory* shareMem, char* filename)
{
    FILE* inFile;
    int c, count = 0;

    inFile = fopen(filename, "r");
    if (inFile == NULL)
    {
        fprintf(stderr, "Error in opening file\n");
        return -1;
    }
    while((c = fgetc(inFile)) != EOF)
    {
        if (((char)c == '\n') || ((char)c == ' '))
        {
            /* Do Nothing/Ignore */
        }
        else
        {
            c -= 48;
            if (c < 1 || c > 9)
            {
                fprintf(stderr, "Error - Sudoku Solution input is invalid\n");
                return -1;
            }
            shareMem->buffer1[0][count] = c;
            count++;
            if (count > 81)
            {
                fprintf(stderr, "Error - Sudoku Solution input is invalid\n");
                return -1;
            }
        }
    }
    if (count != 81)
    {
        fprintf(stderr, "Error - Sudoku Solution input is invalid\n");
        fclose(inFile);
        return -1;
    }
    fclose(inFile);
    return 0;
}

/* Function prints out the results for the program */
void printResults (SharedMemory* shareMem)
{
    int i;

    for (i = 1; i < 10; i++)
    {
        if(shareMem->buffer2[i-1] == 1)
        {
            printf("Validation result from Thread ID-%d: row %d is valid.\n", i, i);
        }
        else
        {
            printf("Validation result from Thread ID-%d: row %d is not valid.\n", i, i);
        }
    }
    printf("Validation result from Thread ID-10: %d of 9 columns are valid.\n", shareMem->buffer2[9]);
    printf("Validation result from Thread ID-11: %d of 9 Sub-Grids are valid.\n\n", shareMem->buffer2[10]);

    if(shareMem->totalVal != 27)
    {
        printf("There are %d valid sub-grids, and thus solution is invalid.\n", shareMem->totalVal);
    }
    else
    {
        printf("There are 27 valid sub-grids, and thus solution is valid.\n");
    }
}
