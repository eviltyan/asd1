// Quicksort.cpp
#include "Quicksort.h"

void Sort(std::vector<int>& arr, int low, int high);
int Partition(std::vector<int>& arr, int low, int high);

// Быстрая сортировка
std::vector<int> Quicksort(std::vector<int> A)
{
    Sort(A, 0, A.size() - 1);
    return A;
}

// Логика сортировки
void Sort(std::vector<int>& arr, int start, int end) 
{
    if (start < end)
    {
        int prop_idx = Partition(arr, start, end);

        Sort(arr, start, prop_idx - 1);
        Sort(arr, prop_idx + 1, end);
    }
}

// Разделение на две части
int Partition(std::vector<int>& arr, int start, int end)
{
    int prop = arr[end];

    int i = start - 1;

    for (int j = start; j < end; j++)
    {
        if (arr[j] <= prop)
        {
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }

    int tmp = arr[i + 1];
    arr[i + 1] = arr[end];
    arr[end] = tmp;

    return i + 1;
}