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
SharedMemory shareMem;

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
                pthread_create(threadArray[i], NULL, groupOne, NULL);
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
    }
}

int groupOne (void* threadNum)
{

}
