#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

void swap(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
    return;
}

void heap_insert(int a[], int new_num, int &l, int b[])
{
    l ++;
    a[l] = new_num;
    b[l] = 0;
    int i = l;
    while (i > 0 && a[i] < a[(i - 1) / 2])
    {
        swap(a[i], a[(i - 1) / 2]);
        swap(b[i], b[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    return;
}

void heap_get(int a[], int b[], int &l)
{
    swap(a[0], a[l]);
    int i = 0;
    while (i * 2 + 1 < l && (a[i] > a[i * 2 + 1] || (i * 2 + 2 < l && a[i] > a[i * 2 + 2])))
    {
        if (a[i * 2 + 1 ] < a[i * 2 + 2] || i * 2 + 2 >= l)
        {
            swap(a[i], a[i * 2 + 1]);
            swap(b[i], b[i * 2 + 1]);
            i = i * 2 + 1;
        }
        else
        {
            swap(a[i], a[i * 2 + 2]);
            swap(b[i], b[i * 2 + 2]);
            i = i * 2 + 2;
        }
    }
    l --;

    swap(a[0], a[l]);
    i = 0;
    while (i * 2 + 1 < l && (a[i] > a[i * 2 + 1] || (i * 2 + 2 < l && a[i] > a[i * 2 + 2])))
    {
        if (a[i * 2 + 1] < a[i * 2 + 2] || i * 2 + 2 >= l)
        {
            swap(a[i], a[i * 2 + 1]);
            swap(b[i], b[i * 2 + 1]);
            i = i * 2 + 1;
        }
        else
        {
            swap(a[i], a[i * 2 + 2]);
            swap(b[i], b[i * 2 + 2]);
            i = i * 2 + 2;
        }
    }

    a[l] = a[l + 1] + a[l];
    b[l] = b[l + 1] + b[l] + a[l]; // calculate total length
    i = l;
    while (i > 0 && a[i] < a[(i - 1) / 2])
    {
        swap(a[i], a[(i - 1) / 2]);
        swap(b[i], b[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string str;
    cin >> str;
    if (str.size() == 1)
    {
        printf("1\n");
        fclose(stdin);
        fclose(stdout);
    }
    int hash_char[128];
    memset(hash_char, 0, 128 * sizeof(int));
    for (int i = 0; i < str.size(); i++)
    {
        hash_char[str[i]] ++;
    }
    int heap_char[100], accomp[100]; // accompany: another array to calculate total length
    int l = -1; // the index of the last num in heap; total number == l + 1
    for (int i = 0; i < 128; i++) // build a heap
    {
        if (hash_char[i] != 0)
        {
            heap_insert(heap_char, hash_char[i], l, accomp);
        }
    }

    while (l > 0) // heap has more than 1 number
    {
        heap_get(heap_char, accomp, l);
    }
    printf("%d\n", accomp[0]);
    fclose(stdin);
    fclose(stdout);
    return 0;
}