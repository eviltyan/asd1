// List.h
#include <iostream>
#include <vector>
#include <string>

typedef struct Edge;
typedef struct Tree;

class List
{
public:
    List(std::string wrd);

private:
    std::string word; // Слово
    int count_leaf = 0; // Номер листа

    Tree* Create_Tree(int number_leaf);
    Edge* Create_Edge(int start, int end, Tree* child = NULL);
    void Add_Edge(Tree* node, Edge* edge);
    Edge* Find_Edge(Tree* node, char chr);
    Tree* Split_Edge(Tree* parent, Edge* edge, int split_pos);
    void Insert_Suffix(Tree* root, int start, int end);
    std::string Get_Edge_String(Edge* edge);
    void Print_Tree(Tree* node, int depth = 0);
    void Find_Suffixes(Tree* node, std::string suffix_path, std::vector<std::string>& suffixes);
    void Free_Edge(Edge* edge);
    void Free_Tree(Tree* node);
};
