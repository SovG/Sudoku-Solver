/* File contains functions pertaining to the validation of each sub-grid */

int validateRow (int rowNum, Buffer1 buffer1, Buffer2 buffer2)
{
    int valid = 1;  /* 1 signifies true, 0 false */
    int i, boxVal;
    int validRow[9];

    for (i = 0; i < 9; i++)
    {
        boxVal = buffer1->sudokuSolution[rowNum][i]
        case boxVal
            case 1:
                validRow[0] = 1;
            case 2:
                validRow[1] = 2;
            case 3:
                validRow[2] = 3;
            case 4:
                validRow[3] = 4;
            case 5:
                validRow[4] = 5;
            case 6:
                validRow[5] = 6;
            case 7:
                validRow[6] = 7;
            case 8:
                validRow[7] = 8;
            case 9:
                validRow[8] = 9;
            default:
                return -1;
    }

    for (i = 0; i < 9; i++)
    {
        if (validRow[i] != i+1)
        {
            valid = 0;
        }
    }
    buffer2->validated[rowNum] = valid;
    return valid;
}

int validateAllCols (Buffer1 buffer1, Buffer2 buffer2)
{
    int i, totalVal;
    for (i = 0; i < 9; i++)
    {
        if ((validateCols(i, buffer1)) == 1)
        {
            totalVal++;
        }
    }

    buffer2->validated[9] = totalVal;
    return totalVal;
}

int validateCols (int colNum, Buffer1 buffer1)
{
    int validRow[9];
    int i, boxVal;

    for (i = 0; i < 9; i++)
    {
        boxVal = buffer1->sudokuSolution[i][colNum];
        case boxVal
            case 1:
                validRow[0] = 1;
            case 2:
                validRow[1] = 2;
            case 3:
                validRow[2] = 3;
            case 4:
                validRow[3] = 4;
            case 5:
                validRow[4] = 5;
            case 6:
                validRow[5] = 6;
            case 7:
                validRow[6] = 7;
            case 8:
                validRow[7] = 8;
            case 9:
                validRow[8] = 9;
            default:
            /* Error, solution can't have a number thats not 1-9 */
                return -1;
    }

    for (i = 0; i < 9; i++)
    {
        if (validRow[i] != i+1)
        {
            valid = 0;
        }
    }

    return valid;

}
