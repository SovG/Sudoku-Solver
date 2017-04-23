/* File contains functions pertaining to File IO for msvv.c */
#import "msvv.h"
#import <stdio.h>

/* Function reads the Sudoku Solution from file and loads it into Buffer 1 */
int readFile (Buffer1* buffer1, char* filename)
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
        if (c < 1 || c > 9)
        {
            fprintf(stderr, "Error - Sudoku Solution input is invalid\n", );
            return -1;
        }
        buffer1->sudokuSolution[0][count] = c;
        count++;
    }
    return 0;
}
