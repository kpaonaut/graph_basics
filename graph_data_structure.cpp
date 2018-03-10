#include <cstdio>
#include <iostream>
using namespace std;

int main()
{
    FILE *fin, *fout;
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    int n;
    fscanf(fin, "%d", &n);

    // votex-based matrix
    int a[100][100] = {0}, tot = 0;
    int e_start[10000], e_end[10000], e_length[10000];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            fscanf(fin, "%d", &a[i][j]);
            if (a[i][j] != 0)
            {
                e_start[tot] = i + 1; // node number, not index
                e_end[tot] = j + 1;
                e_length[tot] = a[i][j];
                tot++;
            }
        }

    // edge-based matrix
    // fprintf(fout, "edge-based martix\n");
    int b[100][10000] = {0};
    for (int i = 0; i < tot; i++)
    {
        b[e_start[i] - 1][i] = 1;
        b[e_end[i] - 1][i] = -1;
    }
    if (tot > 0)
    {
        for (int i = 0; i < n; i++)
        {
            fprintf(fout, "%d", b[i][0]);
            for (int j = 1; j < tot; j++)
            {
                fprintf(fout, " %d", b[i][j]);
            }
            fprintf(fout, "\n");
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
            fprintf(fout, "\n");
    }

    // edge table
    // fprintf(fout, "edge table\n");
    if (tot > 0)
    {
        for (int i = 0; i < tot - 1; i++)
            fprintf(fout, "%d ", e_start[i]);
        fprintf(fout, "%d\n", e_start[tot - 1]);
        for (int i = 0; i < tot - 1; i++)
            fprintf(fout, "%d ", e_end[i]);
        fprintf(fout, "%d\n", e_end[tot - 1]);
        for (int i = 0; i < tot - 1; i++)
            fprintf(fout, "%d ", e_length[i]);
        fprintf(fout, "%d\n", e_length[tot - 1]);
    }
    else fprintf(fout, "\n\n\n");

    // positive-direction table
    // fprintf(fout, "positive-direction table\n");
    int i = 0, current_start = 0;
    int index[101] = {0}, no[10000] = {0};
    index[0] = 1;
    while (i < tot)
    {
        if (current_start != e_start[i]-1)
        {
            int j = current_start + 1;
            while (j <= e_start[i]-1)
            {
                index[j] = i + 1; // the index in C++ starts from 0
                j++;
            }
            current_start = e_start[i] - 1;
        }
        no[i] = e_end[i];
        i++;
    }
    if (tot == 0)
    {
        int j = 0;
        while (j < n)
            {
                index[j] = 1; // the index in C++ starts from 0
                j++;
            }
    }
    index[n] = tot + 1;
    for (int i = 0; i < n; i ++)
    {
        fprintf(fout, "%d ", index[i]);
    }
    fprintf(fout, "%d\n", index[n]);
    for (int i = 0; i < tot-1; i++)
    {
        fprintf(fout, "%d ", no[i]);
    }
    if (tot == 0) 
        fprintf(fout, "\n");
    else
        fprintf(fout, "%d\n", no[tot - 1]);
    for (int i = 0; i < tot - 1; i++)
        fprintf(fout, "%d ", e_length[i]);
    if (tot == 0) 
        fprintf(fout, "\n");
    else
        fprintf(fout, "%d\n", e_length[tot - 1]);


    // adjacent link
    // fprintf(fout, "adjacent link\n");
    bool first = 0;
    for (int i = 0; i < n; i++)
    {
        first = 0;
        for (int j = 0; j < n; j++)
        {
            if (a[i][j] != 0)
            {
                if (first == 0) // first edge
                    fprintf(fout, "%d %d", a[i][j], j+1);
                else // not the first edge
                    fprintf(fout, " %d %d", a[i][j], j+1);
                first = 1;
            }
        }
        fprintf(fout, "\n");
    }

    fclose(fin);
    fclose(fout);
}