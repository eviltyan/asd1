// SelectionMethod.cpp
#include "SelectionMethod.h"

// Сортировка выбора
std::vector<int> Selection_Method(std::vector<int> A)
{
	for (int i = 0; i < A.size() - 1; i++)
	{
		int mi = i;

		for (int j = i + 1; j < A.size(); j++)
		{
			if (A[j] < A[mi])
			{
				mi = j;
			}
		}

		int tmp = A[i];
		A[i] = A[mi];
		A[mi] = tmp;
	}

	return A;
}