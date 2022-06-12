#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pthread.h>

int final = 0;

typedef struct
{
    int row;
    int col;
    int arr[9][9];
} par;

void *rowchecker(void *arg)
{
    // printf("Row\n");
    int temp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int ans = 0;
    int index = 0;
    par *arr = arg;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            //   printf("%d ", arr->arr[i][j]);
            for (int k = 0; k < 9; k++)
            {

                if (arr->arr[i][j] == temp[k])
                {
                    ans = 1;
                    final += ans;
                    return NULL;
                }
            }
            temp[index++] = arr->arr[i][j];
        }
        //  printf("\n");
        for (int j = 0; j < 9; j++)
        {
            temp[j] = 0;
        }
        index = 0;
    }
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
            //      printf("%d ", arr->arr[j][i]);
            for (int k = 0; k < 9; k++)
            {

                if (arr->arr[j][i] == temp[k])
                {
                    ans = 1;
                    final += ans;
                    return NULL;
                }
            }
            temp[index++] = arr->arr[j][i];
        }
        // printf("\n");
        for (int j = 0; j < 9; j++)
        {
            temp[j] = 0;
        }
        index = 0;
    }
    return NULL;
}

void *threebythree(void *arg)
{
    int temp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int index = 0;

    par *arr = arg;

    for (int i = arr->row; i < arr->row + 3; i++)
    {
        for (int j = arr->col; j < arr->col + 3; j++)
        {
            //  printf("%d ", arr->arr[i][j]);
            for (int k = 0; k < 9; k++)
            {
                if (arr->arr[i][j] == temp[k])
                {
                    final += 1;
                    return NULL;
                }
            }
            temp[index++] = arr->arr[i][j];
        }
        //  printf("\n");
    }
    // printf("Print 3by3\n");
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

        pthread_t workers[11];

        pthread_create(&workers[0], NULL, &rowchecker, rowdata);
        pthread_create(&workers[1], NULL, &colchecker, rowdata);
        int colval = 0;
        int rowval = 0;
        for (int i = 2; i < 11; i++)
        {
            par *data = (par *)malloc(sizeof(par));
            for (int j = 0; j < 9; j++)
            {
                for (int k = 0; k < 9; k++)
                {
                    data->arr[j][k] = arr[j][k];
                }
            }
            data->row = rowval;
            data->col = colval;
            // printf("COL:%d ROW:%d\n",data->col,data->row);
            pthread_create(&workers[i], NULL, &threebythree, data);
            if (colval == 6)
            {
                colval = 0;
                rowval += 3;
            }
            else
            {
                colval += 3;
            }
        }

        for (int i = 0; i < 11; i++)
        {
            pthread_join(workers[i], NULL);
        }

        printf("Sudoku Puzzle input is:\n");

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                printf("%d ", arr[i][j]);
            }
            printf("\n");
        }

        if (final > 0)
        {
            printf("Soduku puzzle is invalid\n");
        }
        else
        {
            printf("Soduku puzzle is valid\n");
        }
    }
    return 0;
}