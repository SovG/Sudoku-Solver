/* C file contains the main file for the Multitasked Sudoku Solution Validator
 * as created for the OS (COMP2006) 2017 assignment
 *
 * Created by Nehal Ghuman
* 20th April 2017                                                             */

#import <stdio.h>
#import "memAlloc.h"
#import "msvv.h"

int main (int argc, char* argv[])
{
    if (argc != 4)
    {
        printf ("Please use program in following format\n");
        printf ("msvv INFILE OUTFILE")
    }
    Buffer1 *buffer1;
    Buffer2 *buffer2;
    int* counter;
    int status; /* For error checking */
    /* File Descriptors */
    int buffer1FD, buffer2FD, counterFD;

    buffer1FD = shm_open("Buffer 1", O_CREAT | O_RDRW, 0666);
    buffer2FD = shm_open("Buffer 2", O_CREAT | O_RDRW, 0666);
    counterFD = shm_open("Counter", O_CREAT | O_RDRW, 0666);

    /* Error detection if memory blocks not created properly */
    if ((buffer1FD == -1) || (buffer2FD == -1) || (counterFD == -1))
    {
        fprintf (stderr, "Error Creating shared memory blocks\n");
        return -1;
    }

    status = ftruncate(buffer1FD, sizeof(Buffer1));
    status += ftruncate(buffer2FD, sizeof(Buffer2));
    status += ftruncate(counterFD, sizeof(int));
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

    status = readFile(buffer1, buffer2, filename);



    return 0;
}

/* Buffered1 is shared memory, should be read only, contains the sudoku Solution
 * Buffered2 is shared memory, contains the results of each sub-grid
*/
