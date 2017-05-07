/* Function Prototypes for validate.c */

int validateRow (int rowNum, SharedMemory* shareMem);
int validateAllCols (SharedMemory* shareMem);
int validateCols (int colNum, SharedMemory* shareMem);
int validateAllGrids (SharedMemory* shareMem);
int validateSubGrid (int *subGrid);
