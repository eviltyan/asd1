// RadixSort.cpp
#include "RadixSort.h"

// Поразрядная сортировка
std::vector<int> Radix_Sort(std::vector<int> A)
{
    int max_val = *std::max_element(A.begin(), A.end());

    for (int exp = 1; max_val / exp > 0; exp *= 10) 
    {
        Bucket_Sort(A, exp);
    }

    return A;
}

// Карманная сортировка
void Bucket_Sort(std::vector<int>& A, int exp) 
{
    std::vector<std::vector<int>> buckets(10);

    for (int num : A) 
    {
        int digit = (num / exp) % 10;
        buckets[digit].push_back(num);
    }

    int index = 0;
    for (int i = 0; i < 10; i++) 
    {
        for (int num : buckets[i]) 
        {
            A[index++] = num;
        }
    }
}