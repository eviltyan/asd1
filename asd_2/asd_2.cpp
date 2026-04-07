#include "Header.h"

Tree* Create_Tree(int number_leaf);
Edge* Create_Edge(int start, int end, Tree* child = NULL);
void Add_Edge(Tree* node, Edge* edge);
Edge* Find_Edge(Tree* node, char chr);
Tree* Split_Edge(Tree* parent, Edge* edge, int splitPos);
void Insert_Suffix(Tree* root, int start, int end);
std::string Get_Edge_String(Edge* edge);
void Print_Tree(Tree* node, int depth = 0);
void Find_Suffixes(Tree* node, std::string suffix_path, std::vector<std::string>& suffixes);
void Free_Edge(Edge* edge);
void Free_Tree(Tree* node);


int main()
{
    word = "acacg";
    std::cout << "Строка: \"" << word << "\"" << std::endl;

    int end = word.length();

    std::cout << std::endl << "ОЖИДАЕМЫЕ СУФФИКСЫ" << std::endl;

    for (int i = 0; i < end; i++)
    {
        std::string suffix;
        for (int j = i; j < end; j++)
        {
            suffix += word[j];
        }

        std::cout << "\"" << suffix << "\"" << std::endl;
    }

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

    std::string end_program;
    std::cin >> end_program;

    return 0;
}



// Создание узла
Tree* Create_Tree(int number_leaf)
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
Edge* Create_Edge(int start, int end, Tree* child)
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
void Add_Edge(Tree* node, Edge* edge)
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
Edge* Find_Edge(Tree* node, char chr)
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
Tree* Split_Edge(Tree* parent, Edge* edge, int splitPos)
{
    Tree* split = Create_Tree(-1);

    Edge* first_part = Create_Edge(edge->start, splitPos, split);
    Edge* second_part = Create_Edge(splitPos, edge->end, edge->child);

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
void Insert_Suffix(Tree* root, int start, int end)
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
            // Разделяем ребро и помечаем промежуточный узел как лист
            int split_pos = i;
            Tree* split = Split_Edge(root, edge, split_pos);
            split->number_leaf = ++count_leaf;
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
std::string Get_Edge_String(Edge* edge)
{
    std::string str;

    for (int i = edge->start; i < edge->end && i < (int)word.length(); i++)
    {
        str += word[i];
    }

    return str;
}

// Вывод дерева
void Print_Tree(Tree* node, int depth)
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

// Сбор всех суффиксов в дереве
void Find_Suffixes(Tree* node, std::string suffix_path, std::vector<std::string>& suffixes)
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
void Free_Edge(Edge* edge)
{
    if (!edge) return;

    if (edge->child)
    {
        Free_Tree(edge->child);
    }

    free(edge);
}

// Освобождение памяти узла дерева
void Free_Tree(Tree* node)
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
