// InsertMethod.cpp
#include "InsertMethod.h"

// Сортировка вставкой
std::vector<int> Insert_Method(std::vector<int> A)
{
	for (int i = 1; i < A.size(); i++)
	{
		int j = i;

		while (j > 0 && A[j] < A[j - 1])
		{
			int tmp = A[j];
			A[j] = A[j - 1];
			A[j - 1] = tmp;
			j -= 1;
		}
	}

	return A;
}