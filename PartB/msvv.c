/* File contains main function for msvv program as required for COMP2006
 * Assignment. This file is for part B, so it validates a Sudoku Soltion using
 * multiple-threads.
 *
 * Created by Nehal Ghuman
 * 2nd May, 2017                                                              */

#include "msvv.h"
#include "fileIO.h"
#include "validate.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* Global Variable shared between threads as used by textbook */
SharedMemory* shareMem;
Locks* locks;
int completedRows, maxDelay;
FILE* outFile;

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf ("Please use program in following format\n");
        printf ("msvv INFILE MAXDELAY\n");
    }
    else
    {
        int i, status;
        pthread_t threadArray[11]; /*So we can Join threads after they've completed execution */
        shareMem = (SharedMemory*)malloc(sizeof(SharedMemory));
        locks = (Locks*)malloc(sizeof(Locks));
        /* Initializing default values for variables and Mutex/Condition */
        pthread_mutex_init(&locks->mutex, NULL);
        pthread_cond_init(&locks->complete, NULL);
        completedRows = 0;
        shareMem->totalVal = 0;
        shareMem->completedChildren = 0;
        /* Ensure that logfile has nothing in it */
        outFile = fopen("logfile.txt", "w");
        fclose(outFile);
        status = readFile(shareMem, argv[1]);
        if (status == -1)
        {
            fprintf(stderr, "Error with reading file.\n");
            return -1;
        }
        srand(time(NULL));
        maxDelay = atoi(argv[2]);

        /* Use a FOR Loop to create the 11 Child threads. If it is the first
         * 9 threads being created send it to groupOne, 10th thread goes to
         * groupTwo, and the last thread goes to groupThree                 */
        for (i = 0; i < 11; i++)
        {
            if (i < 9)
            {
                pthread_create(&threadArray[i], NULL, &groupOne, NULL);
            }
            else if (i == 9)
            {
                pthread_create(&threadArray[i], NULL, &groupTwo, NULL);
            }
            else
            {
                pthread_create(&threadArray[i], NULL, &groupThree, NULL);
            }
        }
        /* Waits Parent Thread using pthread_cond_wait() until its signaled
         * by one of its Child threads                                      */
        pthread_cond_wait(&locks->complete, &locks->mutex);
        printResults(shareMem);
        /* Terminates all the Child Threads after all validation has occured */
        for (i = 0; i < 11; i++)
        {
            pthread_join(threadArray[i], NULL);
        }
        /* Free Malloc'd Memory and destroy locks */
        pthread_mutex_destroy(&locks->mutex);
        pthread_cond_destroy(&locks->complete);
        free(locks);
        free(shareMem);
    }
    return 0;
}

/* Each thread assigned to groupOne does a single row of the sudoku solution,
 * uses global variabe completedRows to keep track of which thread is doing which
 * row, and access to that variable is done only via mutex and is assigned a local
 * variable to prevent synchronization issues                                   */
void* groupOne (void* threadNum)
{
    int row, valid;
    /* This code segment is for deciding which row this thread will work on. There
     * is a Global Variable for completedRows which is only incremented inside
     * mutex locks and it is assigned to a variable on the stack to ensure there
     * are no double ups for validating the rows                                */
    pthread_mutex_lock(&locks->mutex);
    completedRows++;
    row = completedRows;
    pthread_mutex_unlock(&locks->mutex);
    valid = validateRow(row, shareMem);
    sleep(rand() % maxDelay);
    /* As accessing shared data using Mutex Locks to solve CSP */
    pthread_mutex_lock(&locks->mutex);
    shareMem->buffer2[row-1] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    /* Only write to log-file if row is invalid */
    if (valid == 0)
    {
        outFile = fopen("logfile.txt", "a");
        fprintf(outFile, "Thread ID-%d: row %d is invalid.\n", row, row);
        fclose(outFile);
    }
    /* If all children have been completed signal parent thread to continue */
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks->complete);
    }
    pthread_mutex_unlock(&locks->mutex);

    pthread_exit(NULL);
}

/* Thread assigned to this function validates all the columns for the Sudoku solution.*/
void* groupTwo (void* nothing)
{
    int totVal[9], valid, i;
    valid = validateAllCols(shareMem, totVal);
    sleep(rand() % maxDelay);
    /* As accessing shared data using Mutex Locks to solve CSP */
    pthread_mutex_lock(&locks->mutex);
    shareMem->buffer2[9] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    /* If total valid columns is not 9 then write to logfile */
    if (valid != 9)
    {
        outFile = fopen("logfile.txt", "a");
        fprintf(outFile, "Thread ID-10: Column, ");
        for (i = 0; i < 9; i++)
        {
            if (totVal[i] == 0)
            {
                fprintf(outFile, "%d, ", i+1);
            }
        }
        fprintf(outFile, "are invalid\n");
        fclose(outFile);
    }
    /* If all children have been completed signal parent thread to continue */
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks->complete);
    }
    pthread_mutex_unlock(&locks->mutex);

    pthread_exit(NULL);
}
/* Threads assigned to this function validates every subgrid of the Sudoku Solution */
void* groupThree (void* nothing)
{
    int totVal[9], valid, i;
    valid = validateAllGrids(shareMem, totVal);
    sleep(rand() % maxDelay);
    /* As accessing shared data using Mutex Locks to solve CSP */
    pthread_mutex_lock(&locks->mutex);
    shareMem->buffer2[10] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    if (valid != 9)
    {
        outFile = fopen("logfile.txt", "a");
        fprintf(outFile, "Thread ID-11: Sub-Grid [");
        for (i = 0; i < 9; i++)
        {
            if (totVal[i] == 0)
            {
                fprintf(outFile, " %d,", i+1);
            }
        }
        fprintf(outFile, " ], are invalid.\n");
        fclose(outFile);
    }
    /* If all children have been completed signal parent thread to continue */
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks->complete);
    }
    pthread_mutex_unlock(&locks->mutex);

    pthread_exit(NULL);
}
