// Header.h
#include <iostream>
#include <vector>
#include <string>

// Ребро
typedef struct Edge
{
    int start;
    int end;
    struct Edge* next;
    struct Tree* child;
} Edge;

// Дерево
typedef struct Tree
{
    int number_leaf;
    Edge* edge_begin;
} Tree;

std::string word; // Слово
int count_leaf = 0; // Номер листа
