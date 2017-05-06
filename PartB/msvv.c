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
Locks* locks;

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf ("Please use program in following format\n");
        printf ("msvv INFILE MAXDELAY");
    }
    else
    {
        pthread_t threadArray[11];
        shareMem = (SharedMemory*)malloc(sizeof(SharedMemory));
        readFile(shareMem, argv[1]);

        int i;
        for (i = 0; i < 11; i++)
        {
            if (i < 9)
            {
                pthread_create(threadArray[i], NULL, groupOne, (void*)i);
            }
            else if (i == 9)
            {
                pthread_create(threadArray[i], NULL, groupTwo, NULL);
            }
            else
            {
                pthread_create(threadArray[i], NULL, groupThree, NULL);
            }
        }
        /* Waits Parent Thread using pthread_cond_wait() until its signaled
         * by one of its Child threads                                      */
        /*TODO: Check if can just have pthread_cond_wait() by itself outside of while loop */
        while ( shareMem->completedChildren < 10)
        {
            pthread_cond_wait();
        }
        printResults(shareMem);

        /*TODO: Check how to properly do pthread_join */
        for (i = 0; i < 11; i++)
        {
            pthread_join(threadArray[i], NULL);
        }

        free(shareMem);
    }
}

int groupOne (void* threadNum)
{
    int row, valid;
    row = (int)threadNum;
    valid = validateRow(row, shareMem->buffer1);

    /* As accessing shared data using Mutex Locks to solve CSP      */
    pthread_mutex_lock(&locks.mutex);
    shareMem->buffer2[row] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    /* If all children have been completed signal parent thread to continue */
    if (shareMem->completedChildren == 10)
    {
        pthread_cond_signal(&locks.empty);
    }
    pthread_mutex_unlock(&locks.mutex);
}

int groupTwo ()
{
    int valid;
    valid = validateAllCols(shareMem->buffer1);

    pthread_mutex_lock(&locks->mutex)
    shareMem->buffer2[9] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks.empty);
    }
    pthread_mutex_unlock(&locks.mutex);
}

int groupThree ()
{
    int valid;
    valid = validateAllGrids(shareMem->buffer1);

    pthread_mutex_lock(&locks->mutex)
    shareMem->buffer2[10] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks.empty);
    }
    pthread_mutex_unlock(&locks.mutex);
}
