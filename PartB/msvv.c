/* File contains main function for msvv program as required for COMP2006
 * Assignment. This file is for part B, so it validates a Sudoku Soltion using
 * multiple-threads.
 *
 * Created by Nehal Ghuman
 * 2nd May, 2017                                                              */

#include "msvv.h";
#include "fileIO.h";
#include <pthread.h>;
/* Global Variable shared between threads as used by textbook */
SharedMemory* shareMem;

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf ("Please use program in following format\n");
        printf ("msvv INFILE MAXDELAY");
    }
    else
    {
        pthread_t* threadArray;
        shareMem = (SharedMemory*)malloc(sizeof(SharedMemory));
        readFile(shareMem, argv[1]);

        int i;
        for (i = 0; i < 10; i++)
        {
            if (i < 8)
            {
                pthread_create(threadArray[i], NULL, groupOne, (void*)i);
                /*Increment completedChildren */
            }
            else if (i == 8)
            {
                pthread_create(threadArray[i], NULL, groupTwo, NULL);
            }
            else
            {
                pthread_create(threadArray[i], NULL, groupThree, NULL);
            }
        }
        /*TODO: Put pthread_cond_wait here to block parent until all children have completed */
        printResults(shareMem);
    }
}

int groupOne (void* threadNum)
{
    int row, valid;

    row =

    valid = validateRow(row, shareMem->buffer1);
    /*TODO: Mutex here for when your manipulating the total val/buffer2 */
    shareMem->buffer2[row] = valid;
    shareMem->totalVal += valid;
}

int groupTwo ()
{
    int valid;

    valid = validateAllCols(shareMem->buffer1);
    /*TODO: Mutext here for when manipulating totalVal/Buffer2 */
    shareMem->buffer2[9] = valid;
    shareMem->totalVal += valid;
}

int groupThree ()
{
    int valid;

    valid = validateAllGrids(shareMem->buffer1);

    shareMem->buffer2[10] = valid;
    shareMem->totalVal += valid;
}
