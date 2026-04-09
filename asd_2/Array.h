// Array.h
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <chrono>

typedef struct Edge;
typedef struct Tree;

class Array
{
private:
    int suffix_count = 0; // Количество найденный суффиксов
    int repeat = 0; // Количество повторяющихся суффиксов

    int branching_count; // Количество ветвлений
    int internal_nodes_count; // Количество внутренних вершин
    int total_edges_sum; // Сумма исходящих ребер из внутренних вершин

    std::string word; // Слово
    int count_leaf = 0; // Номер листа

    Tree* Create_Tree(int number_leaf);
    Edge* Create_Edge(int start, int end, Tree* child = NULL);
    void Add_Edge(Tree* node, Edge* edge, char first_char);
    Edge* Find_Edge(Tree* node, char chr);
    Tree* Split_Edge(Tree* parent, Edge* edge, int split_pos, char firstChar);
    void Insert_Suffix(Tree* root, int start, int end);

    std::string Get_Edge_String(Edge* edge);
    void Print_Tree(Tree* node, int depth = 0);
    void Calculate_Statistics(Tree* node, bool is_root = true);
    void Find_Suffixes(Tree* node, std::string suffix_path, std::vector<std::string>& suffixes);

    void Free_Edge(Edge* edge);
    void Free_Tree(Tree* node);

public:
    Array(std::string wrd);
    int Get_Suffix_Count();
};
