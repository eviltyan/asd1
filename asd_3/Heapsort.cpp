// Heapsort.cpp
#include "Heapsort.h"

std::vector<int> Insert(std::vector<int>& S, int v);
int Remove_Min(std::vector<int>& S);

// Пирамидальная сортировка
std::vector<int> Heapsort(std::vector<int> A)
{
	std::vector<int> S(1, 0);

	for (int i = 0; i < A.size(); i++)
	{
		S = Insert(S, A[i]);
	}

	int i = 0;
	while (S[0] > 0)
	{
		A[i] = Remove_Min(S);
		i += 1;
	}

	return A;
}

// Вставка в пирамиду
std::vector<int> Insert(std::vector<int>& S, int v)
{
	if (S.size() <= S[0] + 1) S.push_back(v);
	else S[S[0] + 1] = v;
	S[0]++;

	for (int i = S[0]; i > 1; i /= 2)
	{
		if (S[i] >= S[i / 2]) break;
		std::swap(S[i], S[i / 2]);
	}

	return S;
}

// Извлечение минимальнного элемента
int Remove_Min(std::vector<int>& S)
{
	int r = S[1], m;
	S[1] = S[S[0]--];

	for (int i = 1; i <= S[0] / 2;)
	{
		if (((i * 2 + 1) <= S[0]) && S[i * 2] > S[i * 2 + 1])
		{
			m = i * 2 + 1;
		}
		else
		{
			m = i * 2;
		}

		if (S[i] <= S[m]) break;
		std::swap(S[i], S[m]);
		i = m;
	}

	return r;
}