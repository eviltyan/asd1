// List.cpp
#include "List.h"

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


List::List(std::string wrd)
{
    word = wrd;
    int end = word.length();

    std::cout << std::endl << "ДЕРЕВО" << std::endl;

    auto start_array_1 = std::chrono::high_resolution_clock::now();

    Tree* root = Create_Tree(-1);

    for (int i = 0; i < end; i++)
    {
        Insert_Suffix(root, i, end);
    }

    auto end_array_1 = std::chrono::high_resolution_clock::now();
    auto duration_array_1 = std::chrono::duration_cast<std::chrono::microseconds>(end_array_1 - start_array_1);

    //Print_Tree(root, 0);

    branching_count = 0;
    internal_nodes_count = 0;
    total_edges_sum = 0;

    Calculate_Statistics(root);

    std::cout << std::endl << "СТАТИСТИКА ДЕРЕВА" << std::endl;
    std::cout << "Количество ветвлений: " << branching_count << std::endl;
    if (branching_count == 0) branching_count = 1;
    std::cout << "Cреднее число исходящих из вершины ребер: " << total_edges_sum / branching_count << std::endl;

    std::cout << std::endl << "НАЙДЕННЫЕ СУФФИКСЫ" << std::endl;

    auto start_array_2 = std::chrono::high_resolution_clock::now();

    std::vector<std::string> suffixes;
    Find_Suffixes(root, "", suffixes);

    auto end_array_2 = std::chrono::high_resolution_clock::now();
    auto duration_array_2 = std::chrono::duration_cast<std::chrono::microseconds>(end_array_2 - start_array_2);

    std::sort(suffixes.begin(), suffixes.end());
    
    for (size_t i = 0; i < suffixes.size(); i++)
    {
        std::cout << "\"" << suffixes[i] << "\"" << std::endl;
    }

    suffix_count = suffixes.size() + repeat;

    std::cout << std::endl << "ВРЕМЯ ВЫПОЛНЕНИЯ" << std::endl;
    std::cout << "Построение суффиксного дерева: " << duration_array_1.count() << " мкс" << std::endl;
    std::cout << "Построение суффиксного массива: " << duration_array_2.count() << " мкс" << std::endl;

    Free_Tree(root);
}

int List::Get_Suffix_Count()
{
    return suffix_count;
}



// Создание узла
Tree* List::Create_Tree(int number_leaf)
{
    Tree* node = (Tree*)malloc(sizeof(Tree));

    if (node == NULL)
    {
        std::cout << "Не удалось выделить память для создания узла!" << std::endl;
        return NULL;
    }

    node->number_leaf = number_leaf;
    node->edge_begin = NULL;
    return node;
}

// Создание ребра
Edge* List::Create_Edge(int start, int end, Tree* child)
{
    Edge* edge = (Edge*)malloc(sizeof(Edge));

    if (edge == NULL)
    {
        std::cout << "Не удалось выделить память для создания ребра!" << std::endl;
        return NULL;
    }

    edge->start = start;
    edge->end = end;
    edge->next = NULL;
    edge->child = child;
    return edge;
}

// Связывание рёбер
void List::Add_Edge(Tree* node, Edge* edge)
{
    if (!node->edge_begin)
    {
        node->edge_begin = edge;
    }
    else
    {
        Edge* temp = node->edge_begin;

        while (temp->next)
        {
            temp = temp->next;
        }

        temp->next = edge;
    }
}


// Поиск ребра по первому символу
Edge* List::Find_Edge(Tree* node, char chr)
{
    Edge* edge = node->edge_begin;

    while (edge)
    {
        if (word[edge->start] == chr)
        {
            return edge;
        }
        edge = edge->next;
    }

    return NULL;
}

// Разделение ребра
Tree* List::Split_Edge(Tree* parent, Edge* edge, int split_pos)
{
    Tree* split = Create_Tree(-1);

    Edge* first_part = Create_Edge(edge->start, split_pos, split);
    Edge* second_part = Create_Edge(split_pos, edge->end, edge->child);

    Add_Edge(split, second_part);

    Edge* temp = parent->edge_begin;
    Edge* prev = NULL;

    while (temp)
    {
        if (temp == edge)
        {
            if (prev)
            {
                prev->next = first_part;
            }
            else
            {
                parent->edge_begin = first_part;
            }

            first_part->next = edge->next;
            free(edge);

            break;
        }

        prev = temp;
        temp = temp->next;
    }

    return split;
}

// Вставка суффикса
void List::Insert_Suffix(Tree* root, int start, int end)
{
    while (start < end)
    {
        Edge* edge = Find_Edge(root, word[start]);

        if (!edge)
        {
            // Нет ребра - создаем новый лист
            Tree* leaf = Create_Tree(++count_leaf);
            Edge* newEdge = Create_Edge(start, end, leaf);
            Add_Edge(root, newEdge);
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
            repeat++;
            return;
        }
        else if (i == edge->end)
        {
            // Полностью прошли ребро, продолжаем в ребенке
            root = edge->child;
            start = j;
            continue;
        }
        else if (j == end)
        {
            // Суффикс закончился внутри ребра
            int split_pos = i;
            Tree* split = Split_Edge(root, edge, split_pos);

            Tree* leaf = Create_Tree(++count_leaf);

            Edge* empty_edge = Create_Edge(end, end, leaf);
            Add_Edge(split, empty_edge);

            return;
        }
        else
        {
            // Частичное совпадение - разделяем ребро
            int split_pos = i;
            Tree* split = Split_Edge(root, edge, split_pos);

            Tree* leaf = Create_Tree(++count_leaf);
            Edge* new_edge = Create_Edge(j, end, leaf);
            Add_Edge(split, new_edge);
            return;
        }
    }
}


// Получение строки на ребре
std::string List::Get_Edge_String(Edge* edge)
{
    std::string str;

    for (int i = edge->start; i < edge->end && i < (int)word.length(); i++)
    {
        str += word[i];
    }

    return str;
}

// Вывод дерева
void List::Print_Tree(Tree* node, int depth)
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
    Edge* edge = node->edge_begin;
    while (edge)
    {
        edges.push_back(edge);
        edge = edge->next;
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

// Подсчёт статистики
void List::Calculate_Statistics(Tree* node, bool is_root)
{
    if (!node) return;

    int edge_count = 0;
    Edge* edge = node->edge_begin;
    while (edge)
    {
        edge_count++;
        edge = edge->next;
    }

    if (!is_root && node->number_leaf == -1 && edge_count > 0)
    {
        internal_nodes_count++;
        total_edges_sum += edge_count;

        if (edge_count > 1)
        {
            branching_count++;
        }
    }

    edge = node->edge_begin;
    while (edge)
    {
        if (edge->child)
        {
            Calculate_Statistics(edge->child, false);
        }
        edge = edge->next;
    }
}

// Сбор всех суффиксов в дереве
void List::Find_Suffixes(Tree* node, std::string suffix_path, std::vector<std::string>& suffixes)
{
    if (!node) return;

    if (node->number_leaf != -1 && suffix_path.length() > 0)
    {
        suffixes.push_back(suffix_path);
        return;
    }

    Edge* edge = node->edge_begin;
    while (edge)
    {
        std::string str = Get_Edge_String(edge);
        if (edge->child)
        {
            Find_Suffixes(edge->child, suffix_path + str, suffixes);
        }
        edge = edge->next;
    }
}


// Освобождение памяти ребра
void List::Free_Edge(Edge* edge)
{
    if (!edge) return;

    if (edge->child)
    {
        Free_Tree(edge->child);
    }

    free(edge);
}

// Освобождение памяти узла дерева
void List::Free_Tree(Tree* node)
{
    if (!node) return;

    Edge* edge = node->edge_begin;
    while (edge)
    {
        Edge* next_edge = edge->next;
        Free_Edge(edge);
        edge = next_edge;
    }

    free(node);
}
