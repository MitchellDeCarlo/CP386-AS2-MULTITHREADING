#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pthread.h>

int finalarr[11];

typedef struct
{
    int row;
    int col;
    int arr[9][9];
} par;

void *rowchecker(void *arg)
{
    int temp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int ans = 0;
    int index = 0;
    par *arr = arg;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 9; k++)
            {

                if (arr->arr[i][j] == temp[k])
                {
                    ans = 1;
                    finalarr[0] = ans;
                    return NULL;
                }
            }
            temp[index++] = arr->arr[i][j];
        }
        for (int j = 0; j < 9; j++)
        {
            temp[j] = 0;
        }
        index = 0;
    }
    finalarr[0] = ans;
    return NULL;
}

void *colchecker(void *arg)
{
    int temp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int ans = 0;
    int index = 0;
    par *arr = arg;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ",arr->arr[j][i]);
            for (int k = 0; k < 9; k++)
            {

                if (arr->arr[j][i] == temp[k])
                {
                    ans = 1;
                    finalarr[1] = ans;
                    return NULL;
                }
            }
            temp[index++] = arr->arr[j][i];
        }
        printf("\n");
        for (int j = 0; j < 9; j++)
        {
            temp[j] = 0;
        }
        index = 0;
    }
    finalarr[1] = ans;
    return NULL;
}

int main(int argc, char *argv[])
{
    int arr[9][9];
    FILE *fp = fopen(argv[1], "r");
    char ch;

    int i = 0;
    int j = 0;
    int checker = 0;

    if (fp == NULL)
    {
        printf("Error opening file.\n");
    }
    else
    {
        while ((ch = fgetc(fp)) != EOF)
        {
            if (isdigit(ch) > 0 && checker != 1)
            {
                arr[i][j] = atoi(&ch);
                j++;

                if (i > 9)
                {
                    j = 0;
                    i++;
                }
            }

            if (checker == 1)
            {
                checker = 0;
            }

            if (ch == '-')
            {
                checker = 1;
            }
        }
        fclose(fp);

        par *rowdata = (par *)malloc(sizeof(par));

        rowdata->row = 0;
        rowdata->col = 0;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                rowdata->arr[i][j] = arr[i][j];
            }
        }
        pthread_t rowcheck;
        pthread_t colcheck;

        pthread_create(&rowcheck, NULL, &rowchecker, rowdata);
        pthread_create(&colcheck, NULL, &colchecker, rowdata);


        pthread_join(rowcheck, NULL);
        pthread_join(colcheck, NULL);
        printf("Row Result %d\n", finalarr[0]);
        printf("Col Result %d\n", finalarr[1]);
    }
    return 0;
}