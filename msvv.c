/* C file contains the main file for the Multitasked Sudoku Solution Validator
 * as created for the OS (COMP2006) 2017 assignment
 *
 * Created by Nehal Ghuman
 * 20th April 2017                                                             */

#import <stdio.h>
#import "memAlloc.h"
#import "msvv.h"
#import <sys/wait.h>
#import <sys/types.h>
#import <semaphore.h>

int main (int argc, char* argv[])
{
    if (argc != 4)
    {
        printf ("Please use program in following format\n");
        printf ("msvv INFILE MAXDELAY")
    }
    Buffer1 *buffer1;
    Buffer2 *buffer2;
    Locks *locks;
    int* counter;

    int parentPID = getpid();   /* So we know which process is parent one */
    int status; /* For error checking */
    int childProcCount = 0; /* Counts how many child processes created, so each child
    knows which row it is working on */
    /* File Descriptors */
    int buffer1FD, buffer2FD, counterFD, locksFD;

    buffer1FD = shm_open("Buffer 1", O_CREAT | O_RDRW, 0666);
    buffer2FD = shm_open("Buffer 2", O_CREAT | O_RDRW, 0666);
    counterFD = shm_open("Counter", O_CREAT | O_RDRW, 0666);
    locksFD = shm_open("Locks", O_CREAT | O_RDRW, 0666);

    /* Error detection if memory blocks not created properly */
    if ((buffer1FD == -1) || (buffer2FD == -1) || (counterFD == -1))
    {
        fprintf (stderr, "Error Creating shared memory blocks\n");
        return -1;
    }

    status = ftruncate(buffer1FD, sizeof(Buffer1));
    status += ftruncate(buffer2FD, sizeof(Buffer2));
    status += ftruncate(counterFD, sizeof(int));
    status += ftruncate(statusFD, sizeof(Locks));
    if (status != 0)
    {
        fprintf(stderr, "Error setting shared memory size\n");
        return -1;
    }


    buffer1 = (Buffer1*)mmap(0, sizeof(Buffer1), PROT_READ | PROT_WRITE, MAP_SHARED,
                                    buffer1FD, 0);
    buffer2 = (Buffer2*)mmap(0, sizeof(Buffer2), PROT_READ | PROT_WRITE, MAP_SHARED,
                                    buffer2FD, 0);
    counter = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED,
                                    counterFD, 0);
    locks = (Locks*)mmap(0, sizeof(Locks), PROT_READ | PROT_WRITE, MAP_SHARED,
                                    locksFD, 0);

    sem_init(locks->mutex, 0, 1);

    status = readFile(buffer1, buffer2, filename);
    if (status == -1)
    {
        fprintf(stderr, "Error with reading file");
        return -1;
    }

    /* Create 9 Children Processes that check validty of each row of solution */
    for (i = 0; i < 11; i++)
    {
        /* Only make a child if process is parent */
        if (parentPID == getpid())
        {
            pid = fork();
            childProcCount++;
        }
    }

    if (parentPID != getpid())
    {
        /* If Child Process is one of the first 9 created then use it to validate
         * an individual row of the solution, where childProcCount tells function
         * what row to validate */
        if(childProcCount < 9)
        {
            validateRow(childProcCount, buffer1, buffer2);
            sem_wait(locks->mutex);
            counter = counter + buffer2->validated[childProcCount];
            sem_post(locks->mutex);
            /* Increment Counter somewhere here if valid True */
        }
        else if (childProcCount == 9)
        {
            vaildateAllCols(buffer1, buffer2);
            sem_wait(locks->mutex);
            counter = counter + buffer2->validated[9];
            sem_post(locks->mutex);
        }
        else
        {
            validateAllGrids(buffer1, buffer2);
            sem_wait(locks->mutex);
            counter = counter + buffer2->validated[10];
            sem_post(locks->mutex);
        }
        exit(0); /* Child Process exits after completing calculations */
    }
    else
    {
        /* Make Parent Process wait for all children to complete */
        while(waitpid(-1, NULL, 0))
        {
            if (errno == ECHILD)
                break;
        }
    }
    /* Print Stuff now that all validation is done */
    printResults(buffer2, counter);

    return 0;
}

/* Buffered1 is shared memory, should be read only, contains the sudoku Solution
 * Buffered2 is shared memory, contains the results of each sub-grid
*/
