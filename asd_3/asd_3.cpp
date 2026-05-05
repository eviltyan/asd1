// asd_3.cpp
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

#include "SelectionMethod.h"
#include "InsertMethod.h"
#include "Quicksort.h"
#include "Heapsort.h"
#include "RadixSort.h"


// Генерация последовательности в прямом порядке
std::vector<int> Generate_Sorted_Ascending(int size) 
{
    int min_val = 1000, max_val = 9999;
    std::vector<int> result(size);

    if (size > 0) 
    {
        double range = static_cast<double>(max_val - min_val);

        for (int i = 0; i < size; i++) 
        {
            double t = static_cast<double>(i) / (size - 1);
            int value = min_val + static_cast<int>(t * range + 0.5);
            value = std::min(max_val, std::max(min_val, value));
            result[i] = value;
        }
    }

    return result;
}

// Генерация последовательности в обратном порядке
std::vector<int> Generate_Sorted_Descending(int size) 
{
    int min_val = 1000, max_val = 9999;
    std::vector<int> result(size);

    if (size > 0) 
    {
        double range = static_cast<double>(max_val - min_val);

        for (int i = 0; i < size; i++) 
        {
            double t = static_cast<double>(size - 1 - i) / (size - 1);
            int value = min_val + static_cast<int>(t * range + 0.5);
            value = std::min(max_val, std::max(min_val, value));
            result[i] = value;
        }
    }

    return result;
}

// Генерация последовательности в случайном порядке
std::vector<int> Generate_Random(int size) 
{
    int min_val = 1000, max_val = 9999;
    std::vector<int> result(size);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(min_val, max_val);

    for (int i = 0; i < size; i++) 
    {
        result[i] = dist(gen);
    }

    return result;
}

// Вывод массива
void Print_Array(std::vector<int> A)
{
    std::cout << std::endl;
    for (int i = 0; i < A.size(); i++) std::cout << A[i] << "; ";
    std::cout << std::endl;
}

// Проверка корректности сортировки
bool Is_Correct(std::vector<int> res, std::vector<int> result)
{
    for (int i = 0; i < result.size(); i++)
    {
        if (result[i] != res[i])
        {
            return false;
        }
    }
    return true;
}

// Тестирование отдельной сортировки
std::vector<int> Sorting(std::vector<int> A, std::vector<int> result, int choice)
{
    std::vector<int> res;

    if (choice == 1)
    {
        auto start = std::chrono::high_resolution_clock::now();
        res = Selection_Method(A);
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Время выполнения сортировки методом выбора: " << time.count() << " мкс" << std::endl;
    }
    else if (choice == 2)
    {
        auto start = std::chrono::high_resolution_clock::now();
        res = Insert_Method(A);
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Время выполнения сортировки методом вставок: " << time.count() << " мкс" << std::endl;
    }
    else if (choice == 3)
    {
        auto start = std::chrono::high_resolution_clock::now();
        res = Quicksort(A);
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Время выполнения быстрой сортировки: " << time.count() << " мкс" << std::endl;
    }
    else if (choice == 4)
    {
        auto start = std::chrono::high_resolution_clock::now();
        res = Heapsort(A);
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Время выполнения пирамидальной сортировки: " << time.count() << " мкс" << std::endl;
    }
    else
    {
        auto start = std::chrono::high_resolution_clock::now();
        res = Radix_Sort(A);
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Время выполнения поразрядной сортировки: " << time.count() << " мкс" << std::endl;
    }

    return res;
}

// Тестирование всех сортировок
int Sorting_Operations(std::vector<int> A, std::vector<int> result)
{
    for (int i = 1; i <= 5; i++)
    {
        std::vector<int> res = Sorting(A, result, i);

        if (!result.empty() && !Is_Correct(res, result))
        {
            switch (i) 
            {
            case 1: std::cout << "Сортировка методом выбора работает неправильно!\n"; return 1;
            case 2: std::cout << "Сортировка методом вставок работает неправильно!\n"; return 1;
            case 3: std::cout << "Быстрая сортировка работает неправильно!\n"; return 1;
            case 4: std::cout << "Пирамидальная сортировка работает неправильно!\n"; return 1;
            case 5: std::cout << "Порязрядная сортировка работает неправильно!\n"; return 1;
            }
        }
    }
    return 0;
}

// Варианты, заданные для тестирования
void Tasks()
{
    for (int i = 0; i < 3; i++)
    {
        int res = 1;
        std::cout << std::endl;

        if (i == 0)
        {
            std::cout << "Вариант 19" << std::endl
                << "35; 4; 0; 8; 759; 505; 41; 9; 8; 7; 856; 47; 841; 740; 4;" << std::endl << std::endl;

            std::vector<int> A = { 35, 4, 0, 8, 759, 505, 41, 9, 8, 7, 856, 47, 841, 740, 4 };
            std::vector<int> result = { 0, 4, 4, 7, 8, 8, 9, 35, 41, 47, 505, 740, 759, 841, 856 };
            res = Sorting_Operations(A, result);
            
        } 
        else if (i == 1)
        {
            std::cout << "Вариант 23" << std::endl
                << "70; 736; 567; 4; 452; 350; 4; 20; 1; 97; 57; 3; 909; 48; 688;" << std::endl << std::endl;

            std::vector<int> A = { 70, 736, 567, 4, 452, 350, 4, 20, 1, 97, 57, 3, 909, 48, 688 };
            std::vector<int> result = { 1, 3, 4, 4, 20, 48, 57, 70, 97, 350, 452, 567, 688, 736, 909 };
            res = Sorting_Operations(A, result);
        }
        else
        {
            std::cout << "Вариант 29" << std::endl
                << "958; 288; 70; 343; 70; 77; 3; 826; 838; 79; 159; 9; 4; 0; 949;" << std::endl << std::endl;

            std::vector<int> A = { 958, 288, 70, 343, 70, 77, 3, 826, 838, 79, 159, 9, 4, 0, 949 };
            std::vector<int> result = { 0, 3, 4, 9, 70, 70, 77, 79, 159, 288, 343, 826, 838, 949, 958 };
            res = Sorting_Operations(A, result);
        }

        if (res == 0) std::cout << "Сортировки выдают правильный результат!" << std::endl;
    }
}

// Меню
void Menu()
{
    int choice = 0, size = 0; std::vector<int> A, _;
    std::cout << std::endl << "Выберите действие: ";
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        Tasks();
        Menu();

    case 2:
        std::cout << std::endl << "Введите размер массива для сортировки: ";
        std::cin >> size;

        if (size <= 0)
        {
            std::cout << "Размер должен быть больше нуля!" << std::endl;
            Menu();
        }

        std::cout << std::endl << "Последовательность, упорядоченная в прямом порядке";
        A = Generate_Sorted_Ascending(size);
        Print_Array(A);
        std::cout << std::endl;
        Sorting_Operations(A, _);
        std::cout ;

        std::cout << std::endl << "Последовательность, упорядоченная в обратном порядке";
        A = Generate_Sorted_Descending(size);
        Print_Array(A);
        std::cout << std::endl;
        Sorting_Operations(A, _);

        std::cout << std::endl << "Cлучайная последовательность";
        A = Generate_Random(size);
        Print_Array(A);
        std::cout << std::endl;
        Sorting_Operations(A, _);

        Menu();

    case 3:
        exit(0);

    default:
        std::cout << "Действие недоступно!" << std::endl;
        Menu();
    }
}

int main()
{
    std::cout << "Доступно:" << std::endl 
        << "1 - Тестирование программы на заданных вариантах" << std::endl
        << "2 - Тестирование алгоритмов на разных последовательностях" << std::endl
        << "3 - Выход" << std::endl;

    Menu();

    return 0;
}
