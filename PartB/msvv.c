/* File contains main function for msvv program as required for COMP2006
 * Assignment. This file is for part B, so it validates a Sudoku Soltion using
 * multiple-threads.
 *
 * Created by Nehal Ghuman
 * 2nd May, 2017                                                              */

 int main (int argc, char* argv[])
 {
     if (argc != 3)
     {
         printf ("Please use program in following format\n");
         printf ("msvv INFILE MAXDELAY");
     }
     else
     {
         int buffer1[9][9];
         int buffer2[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
         int threadsComplete = 0;
         int i;

         for (i = 0; i < 10; i++)
         {
             if (i < 8)
             {
                 
             }
         }
     }
 }
