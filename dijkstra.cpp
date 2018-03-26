#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n, k;
    int a[32][32];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            a[i][j] = 100000; // initialize to inf
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &a[i][j]);
            if (a[i][j] == 0)
                a[i][j] = 100000;
        }
    scanf("%d", &k);
    int visited[32] = {0};
    visited[k - 1] = 1;
    for (int i = 0; i < n; i++)
    {
        int min = 100000;
        int min_i = -1;
        for (int j = 0; j < n; j++)
        {
            if ((visited[j] == 0) && (a[k - 1][j] < min))
            {
                min = a[k - 1][j];
                min_i = j + 1;
            }
        }
        if (min_i == -1) break;
        visited[min_i - 1] = 1;
        // update
        for (int j = 0; j < n; j++)
        {
            if ((visited[j] == 0) && (a[k - 1][min_i - 1] + a[min_i - 1][j] < a[k - 1][j]))
                a[k - 1][j] = a[k - 1][min_i - 1] + a[min_i - 1][j];
        }
    }
    // print
    int check = 0;
    for (int i = 0; i < n; i++)
    {
        if (i != k - 1)
        {
            if (check == 0)
            {
                if (a[k - 1][i] != 100000)
                    printf("%d", a[k - 1][i]);
                else
                    printf("%d", -1);
                check = 1;
            }
            else
            {
                if (a[k - 1][i] != 100000)
                    printf(" %d", a[k - 1][i]);
                else
                    printf(" %d", -1);
            }
        }
    }
    printf("\n");
    fclose(stdin);
    fclose(stdout);
    return 0;
}