/* C file contains the main file for the Multitasked Sudoku Solution Validator
 * as created for the OS (COMP2006) 2017 assignment
 *
 * Created by Nehal Ghuman
 * 20th April 2017                                                             */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include "msvv.h"
#include "validate.h"
#include "fileIO.h"

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf ("Please use program in following format\n");
        printf ("msvv INFILE MAXDELAY");
    }
    else
    {
        Buffer1 *buffer1;
        Buffer2 *buffer2;
        Locks *locks;
        FILE* outFile;
        int* counter;
        int i;
        pid_t parentPID = getpid();   /* So we know which process is parent one */
        int status; /* For error checking */
        int childProcCount = 0; /* Counts how many child processes created, so each child
        knows which row it is working on */
        /* File Descriptors */
        int buffer1FD, buffer2FD, counterFD, locksFD;

        buffer1FD = shm_open("Buffer 1", O_CREAT | O_RDWR, 0666);
        buffer2FD = shm_open("Buffer 2", O_CREAT | O_RDWR, 0666);
        counterFD = shm_open("Count", O_CREAT | O_RDWR, 0666);
        locksFD = shm_open("Lock", O_CREAT | O_RDWR, 0666);

        /* Error detection if memory blocks not created properly */
        if ((buffer1FD == -1) || (buffer2FD == -1) || (counterFD == -1) || (locksFD == -1))
        {
            fprintf (stderr, "Error Creating shared memory blocks\n");
            return -1;
        }

        status = ftruncate(buffer1FD, sizeof(Buffer1));
        status += ftruncate(buffer2FD, sizeof(Buffer2));
        status += ftruncate(counterFD, sizeof(int));
        status += ftruncate(locksFD, sizeof(Locks));
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

        sem_init(&locks->mutex, 1, 1);
        sem_init(&locks->empty, 1, 0);
        buffer2->completedChildren = 0;
        for (i = 0; i < 11; i++)
        {
            buffer2->validated[i] = 0;
        }

        status = readFile(buffer1, argv[1]);
        if (status == -1)
        {
            fprintf(stderr, "Error with reading file");
            return -1;
        }
        *counter = 0;
        /* Ensures LogFile is empty when you start to write to it */
        outFile = fopen("logfile.txt", "w");
        fclose(outFile);
        /* Create 11 Children Processes that check validty of each row of solution */
        for (i = 0; i < 11; i++)
        {
            /* Only make a child if process is parent */
            if (parentPID == getpid())
            {
                childProcCount++;
                fork();
            }

        }

        if (parentPID != getpid())
        {
            /* Each Child Process is seeded by its Process ID that is unique to
             * itself, ensuring that the random sleep is random between childs */
            srand(getpid()+childProcCount);
            /* If Child Process is one of the first 9 created then use it to validate
             * an individual row of the solution, where childProcCount tells function
             * what row to validate */
            if(childProcCount < 10)
            {
                int valid;
                valid = validateRow(childProcCount, buffer1);
                sleep(rand() % atoi(argv[2]));
                sem_wait(&locks->mutex);
                *counter += valid;
                /* Variable completedChildren holds the number of processes completed
                 * so when it reaches 11, the semaphore empty which is blocking the
                 * parent, will be incremented so it can continue execution */
                buffer2->validated[childProcCount-1] = valid;
                buffer2->completedChildren++;
                if (buffer2->completedChildren == 11)
                {
                    sem_post(&locks->empty);
                } 
                if (valid == 0)
                {
                    outFile = fopen("logfile.txt", "a");
                    fprintf(outFile, "Process ID-%d: row %d is invalid.\n", childProcCount, childProcCount);
                    fclose(outFile);
                }   
                sem_post(&locks->mutex);
                
            }
            else if (childProcCount == 10)
            {
                int totVal[9], valid, i;
                valid = validateAllCols(buffer1, totVal);
                sleep(rand() % atoi(argv[2]));
                sem_wait(&locks->mutex);
                *counter += valid;
                buffer2->completedChildren++;
                buffer2->validated[childProcCount-1] = valid;
                if (valid != 9)
                {
                    outFile = fopen("logfile.txt", "a");
                    fprintf(outFile, "Process ID-10: Column, ");
                    for (i = 0; i < 9; i++)
                    {
                        fprintf(outFile, "%d, ", i+1);
                    }
                    fprintf(outFile, "are invalid\n");
                    fclose(outFile);
                }
                if (buffer2->completedChildren == 11)
                {
                    sem_post(&locks->empty);
                }
                sem_post(&locks->mutex);
            }
            else
            {
                int totVal[9], valid, i;
                valid = validateAllGrids(buffer1);
                sleep(rand() % atoi(argv[2]));
                sem_wait(&locks->mutex);
                *counter += valid;
                buffer2->validated[childProcCount-1] = valid;
                buffer2->completedChildren++;
                if (valid != 9)
                {
                    outFile = fopen("logfile.txt", "a");
                    fprintf(outFile, "Process ID-11: SubGrid, [");
                    for (i = 0; i < 9; i++)
                    {
                        fprintf(outFile, " %d,", i+1);
                    }
                    fprintf(outFile, " ] are invalid\n");
                    fclose(outFile);
                }
                if (buffer2->completedChildren == 11)
                {
                    sem_post(&locks->empty);
                }
                sem_post(&locks->mutex);
            }
            exit(0); /* Child Process exits after completing calculations */
        }
        else
        {
            /* Semaphore Empty is only incremented so that it passes this block when
             * the last child process completes. Makes sure that the parent is blocked
             * until all validation is complete                                     */
            sem_wait(&locks->empty);
            /* wait(NULL) kills all the zombie processes as by this stage all
             * tasks to be done by the children have been completed and the
             * parent has been unblocked by the semaphore                     */
            for (i = 0; i < 11; i++)
            {
                wait(NULL);
            }
        }
        /* Print Stuff now that all validation is done */
        printResults(buffer2, *counter); 
        fflush(stdout);

        /* Destroy shared memory and semaphores */
        sem_destroy(&locks->mutex);
        sem_destroy(&locks->empty);

        close(buffer1FD);
        close(buffer2FD);
        close(counterFD);
        close(locksFD);
    }
    return 0;
}
