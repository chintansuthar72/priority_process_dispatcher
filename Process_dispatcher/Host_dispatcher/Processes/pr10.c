#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char alphabets[26][5][6];
    int i, j, k;

    char name[30] = "ABCD";
    strcpy(alphabets[0][1], "*****");
    strcpy(alphabets[0][2], "*   *");
    strcpy(alphabets[0][3], "*****");
    strcpy(alphabets[0][4], "*   *");
    strcpy(alphabets[0][5], "*   *");

    strcpy(alphabets[1][1], "*****");
    strcpy(alphabets[1][2], "*   *");
    strcpy(alphabets[1][3], "*****");
    strcpy(alphabets[1][4], "*   *");
    strcpy(alphabets[1][5], "*****");

    strcpy(alphabets[2][1], "****");
    strcpy(alphabets[2][2], "*   ");
    strcpy(alphabets[2][3], "*   ");
    strcpy(alphabets[2][4], "*   ");
    strcpy(alphabets[2][5], "****");

    strcpy(alphabets[3][1], "    *");
    strcpy(alphabets[3][2], "    *");
    strcpy(alphabets[3][3], "*****");
    strcpy(alphabets[3][4], "*   *");
    strcpy(alphabets[3][5], "*****");

    strcpy(alphabets[4][1], "*****");
    strcpy(alphabets[4][2], "*    ");
    strcpy(alphabets[4][3], "*****");
    strcpy(alphabets[4][4], "*    ");
    strcpy(alphabets[4][5], "*****");

    strcpy(alphabets[5][1], "*****");
    strcpy(alphabets[5][2], "*    ");
    strcpy(alphabets[5][3], "*****");
    strcpy(alphabets[5][4], "*    ");
    strcpy(alphabets[5][5], "*    ");

    for (j = 1; j <= 5; j++)
    {
        k = 0;
        i = (int)toupper(name[k]) - 65;
        while (i < 26)
        {
            printf("%s ", alphabets[i][j]);
            if (name[++k] == '\0')
            {
                break;
            }
            i = (int)toupper(name[k]) - 65;
        }
        printf("\n");
    }

    return 0;
}