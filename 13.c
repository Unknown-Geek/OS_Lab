#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int n, m;
    int row, col, flag, index = 0;
    printf("Enter the number of processes : ");
    scanf("%d", &n);
    printf("\n");
    printf("Enter the number of resources : ");
    scanf("%d", &m);
    printf("\n");
    row = n;
    col = m;

    int alloc[row][col], max[row][col], need[row][col], avail[col], work[col], finish[row], safeseq[row];

    // Allocation matrix
    printf("Enter the allocation matrix :\n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }
    printf("\n");

    // Max matrix
    printf("Enter the Max matrix :\n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }
    printf("\n");

    // Need matrix
    printf("Need Matrix : \n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            need[i][j] = max[i][j] - alloc[i][j];
            printf("%d\t", need[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Available matrix
    printf("Enter the available matrix :\n");
    for (int i = 0; i < col; i++)
    {
        scanf("%d", &avail[i]);
    }
    printf("\n");

    // Work matrix
    for (int i = 0; i < col; i++)
    {
        work[i] = avail[i];
    }

    // Finish matrix initilalisation
    for (int i = 0; i < row; i++)
    {
        finish[i] = 0;
    }

    // Main Logic
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < row; j++)
        {
            if (finish[j] == 0)
            {
                flag = 0;
                for (int k = 0; k < col; k++)
                {
                    if (need[j][k] > work[k])
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    safeseq[index] = j;
                    index++;
                    for (int i = 0; i < col; i++)
                    {
                        work[i] += alloc[j][i];
                    }
                    finish[j] = 1;
                }
            }
        }
    }

    printf("Safe Sequence : \n");
    if (index == n)
    {
        for (int i = 0; i < row; i++)
        {
            printf("%d\t", safeseq[i]);
        }
    }
    else
    {
        printf("Sequence ended in deadlock");
    }
    printf("\n");

    return 0;
}
