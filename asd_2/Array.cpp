// Array.cpp
#include "Array.h"

// Ребро
typedef struct Edge
{
    int start;
    int end;
    struct Tree* child;
} Edge;

// Дерево
typedef struct Tree
{
    int number_leaf;
    Edge* edges[256];
} Tree;


Array::Array(std::string wrd)
{
    word = wrd;
    int end = word.length();

    std::cout << std::endl << "ДЕРЕВО" << std::endl;

    Tree* root = Create_Tree(-1);

    for (int i = 0; i < end; i++)
    {
        Insert_Suffix(root, i, end);        
    }

    Print_Tree(root);

    std::cout << std::endl << "НАЙДЕННЫЕ СУФФИКСЫ" << std::endl;

    std::vector<std::string> suffixes;
    Find_Suffixes(root, "", suffixes);

    for (size_t i = 0; i < suffixes.size(); i++)
    {
        std::cout << "\"" << suffixes[i] << "\"" << std::endl;
    }

    Free_Tree(root);
}

// Создание узла
Tree* Array::Create_Tree(int number_leaf)
{
    Tree* node = (Tree*)malloc(sizeof(Tree));

    if (node == NULL)
    {
        std::cout << "Не удалось выделить память для создания узла!" << std::endl;
        return NULL;
    }

    node->number_leaf = number_leaf;

    // Инициализируем массив нулевыми указателями
    memset(node->edges, 0, sizeof(node->edges));

    return node;
}

// Создание ребра
Edge* Array::Create_Edge(int start, int end, Tree* child)
{
    Edge* edge = (Edge*)malloc(sizeof(Edge));

    if (edge == NULL)
    {
        std::cout << "Не удалось выделить память для создания ребра!" << std::endl;
        return NULL;
    }

    edge->start = start;
    edge->end = end;
    edge->child = child;

    return edge;
}

// Добавление ребра (по индексу символа)
void Array::Add_Edge(Tree* node, Edge* edge, char first_char)
{
    int index = (unsigned char)first_char;
    node->edges[index] = edge;
}

// Поиск ребра по первому символу - O(1)
Edge* Array::Find_Edge(Tree* node, char chr)
{
    int index = (unsigned char)chr;
    return node->edges[index];
}

// Разделение ребра
Tree* Array::Split_Edge(Tree* parent, Edge* edge, int split_pos, char first_char)
{
    Tree* split = Create_Tree(-1);

    Edge* first_part = Create_Edge(edge->start, split_pos, split);
    Edge* second_part = Create_Edge(split_pos, edge->end, edge->child);

    Add_Edge(split, second_part, word[split_pos]);

    parent->edges[(unsigned char)first_char] = first_part;

    free(edge);

    return split;
}

// Вставка суффикса
void Array::Insert_Suffix(Tree* root, int start, int end)
{
    Tree* current = root;

    while (start < end)
    {
        Edge* edge = Find_Edge(current, word[start]);

        if (!edge)
        {
            // Нет ребра - создаем новый лист
            Tree* leaf = Create_Tree(++count_leaf);
            Edge* newEdge = Create_Edge(start, end, leaf);
            Add_Edge(current, newEdge, word[start]);
            return;
        }

        int i = edge->start;
        int j = start;

        while (i < edge->end && j < end && word[i] == word[j])
        {
            i++; j++;
        }

        if (i == edge->end && j == end)
        {
            // Полное совпадение - суффикс уже существует
            return;
        }
        else if (i == edge->end)
        {
            // Полностью прошли ребро, продолжаем в ребенке
            current = edge->child;
            start = j;
            continue;
        }
        else if (j == end)
        {
            // Суффикс закончился внутри ребра
            int split_pos = i;
            Tree* split = Split_Edge(current, edge, split_pos, word[edge->start]);
            split->number_leaf = ++count_leaf;
            return;
        }
        else
        {
            // Частичное совпадение - разделяем ребро
            int split_pos = i;
            Tree* split = Split_Edge(current, edge, split_pos, word[edge->start]);

            Tree* leaf = Create_Tree(++count_leaf);
            Edge* new_edge = Create_Edge(j, end, leaf);
            Add_Edge(split, new_edge, word[j]);
            return;
        }
    }
}

// Получение строки на ребре
std::string Array::Get_Edge_String(Edge* edge)
{
    std::string str;
    for (int i = edge->start; i < edge->end && i < (int)word.length(); i++)
    {
        str += word[i];
    }
    return str;
}

// Вывод дерева
void Array::Print_Tree(Tree* node, int depth)
{
    if (!node) return;

    for (int i = 0; i < depth; i++)
    {
        std::cout << "│   ";
    }

    if (depth == 0)
    {
        std::cout << "■ Корень" << std::endl;
    }
    else if (node->number_leaf != -1)
    {
        std::cout << "■ Лист " << node->number_leaf << std::endl;
        return;
    }
    else
    {
        std::cout << "●" << std::endl;
    }

    std::vector<Edge*> edges;
    for (int i = 0; i < 256; i++)
    {
        if (node->edges[i] != NULL)
        {
            edges.push_back(node->edges[i]);
        }
    }

    for (size_t i = 0; i < edges.size(); i++)
    {
        Edge* edge = edges[i];
        bool is_last = (i == edges.size() - 1);

        for (int j = 0; j < depth; j++)
        {
            std::cout << "│   ";
        }

        if (is_last)
        {
            std::cout << "└── ";
        }
        else
        {
            std::cout << "├── ";
        }

        std::cout << "→ \"" << Get_Edge_String(edge) << "\"";

        if (edge->child && edge->child->number_leaf != -1)
        {
            std::cout << " → Лист " << edge->child->number_leaf;
        }

        std::cout << std::endl;

        if (edge->child)
        {
            Print_Tree(edge->child, depth + 1);
        }
    }
}

// Сбор всех суффиксов
void Array::Find_Suffixes(Tree* node, std::string suffix_path, std::vector<std::string>& suffixes)
{
    if (!node) return;

    if (node->number_leaf != -1 && suffix_path.length() > 0)
    {
        suffixes.push_back(suffix_path);
        return;
    }

    for (int i = 0; i < 256; i++)
    {
        Edge* edge = node->edges[i];
        if (edge && edge->child)
        {
            std::string str = Get_Edge_String(edge);
            Find_Suffixes(edge->child, suffix_path + str, suffixes);
        }
    }
}

// Освобождение памяти
void Array::Free_Edge(Edge* edge)
{
    if (!edge) return;

    if (edge->child)
    {
        Free_Tree(edge->child);
    }

    free(edge);
}

void Array::Free_Tree(Tree* node)
{
    if (!node) return;

    for (int i = 0; i < 256; i++)
    {
        if (node->edges[i])
        {
            Free_Edge(node->edges[i]);
        }
    }

    free(node);
}
