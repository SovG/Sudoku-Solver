/* File contains Prototype Declerations for validate.c */
#include "msvv.h"

int validateRow (int rowNum, Buffer1* buffer1);
int validateAllCols (Buffer1* buffer1, int*);
int validateCols (int colNum, Buffer1* buffer1);
int validateAllGrids (Buffer1* buffer1, int*);
int validateSubGrid (int subgrid[]);
