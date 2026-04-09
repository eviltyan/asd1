# Текст лабораторной работы
Реализовать алгоритм построения суффиксного дерева строки длины n наивным алгоритмом. Добавление суффиксов в дерево начинается с самого длинного, затем на один символ короче и так далее, время работы алгоритма O(n2).  
  
Дерево должно быть сжатым (внутренние вершины - только ветвления, на ребрах несколько символов), метку ребра хранить в виде пары индексов.  
Алфавит - обычный.  
  
Пути из вершин реализовать двумя способами:  
1. Отображение символа в указатель на вершину, реализуется на массиве, работает за константное время.  
2. Вершина хранит указатель на начало односвязного списка. Элемент списка содержит метку ребра и указатель. Поиск пути - обычный перебор списка.  
3. *(необязательно) Вершина хранит пути с помощью дерева поиска.  
  
Протестировать реализации на предмет корректности работы на небольшом примере. Обойти дерево в глубину в алфавитном порядке и вывести на экран все суффиксы (должен получиться полный перечень суффиксов в алфавитном порядке).  
  
Протестировать реализации на предмет производительности.  
1. Оценить зависимость времени построения суффиксного дерева от размера строки, сравнить с теоретической оценкой.   
2. Оценить зависимость времени построения суффиксного массива на основе построенного дерева от размера строки (только обход дерева, без построения).  
Для каждого построенного дерева подсчитывать количество ветвлений и среднее число исходящих из вершины ребер.  
  
При тестировании производительности рассмотреть несколько видов входных данных:  
- искусственно созданные “плохой” и "хороший" случай (много и мало общих префиксов);  
- полностью случайный набор символов;  
- реальный текст.  
  
# Суть задания
Построить дерево, на рёбрах которого будут написаны индексы начальной и конечной позиции суффикса в слове. Сам лист - это суффикс.  
При этом листья реализованы либо с помощью массива, либо с помощью списка.  
Если нарисовать картинку того, чего мы должны сделать, то на примере списка будет что0то такое:  
![alt text](image.png)
  
# Результаты экспериментов (без вывода дерева и ожидаемых суффиксов)
## Хороший случай
![alt text](image-1.png)  
![alt text](image-2.png)  
![alt text](image-3.png)  
![alt text](image-4.png)  
![alt text](image-5.png)  
![alt text](image-6.png)  
![alt text](image-7.png)  
![alt text](image-8.png)  
![alt text](image-9.png)  

## Плохой случай
![alt text](image-10.png)  
![alt text](image-11.png)  
![alt text](image-12.png)  
![alt text](image-13.png)  
![alt text](image-14.png)  
![alt text](image-15.png)  
![alt text](image-16.png)  
![alt text](image-17.png)  
![alt text](image-18.png)  
![alt text](image-19.png)  
![alt text](image-20.png)  
![alt text](image-21.png)  
![alt text](image-22.png)  

## Случайный набор символов
![alt text](image-23.png)  
![alt text](image-24.png)  
![alt text](image-25.png)  
![alt text](image-26.png)  
![alt text](image-27.png)  
![alt text](image-28.png)  
![alt text](image-29.png)  

## Реальный текст
![alt text](image-30.png)  
![alt text](image-31.png)  
![alt text](image-32.png)  
![alt text](image-33.png)  
![alt text](image-34.png)  
![alt text](image-35.png)  
![alt text](image-36.png)  
![alt text](image-37.png)  
![alt text](image-38.png)  
![alt text](image-39.png)  
![alt text](image-40.png)  
![alt text](image-41.png)  
![alt text](image-42.png)  
![alt text](image-43.png)  
![alt text](image-44.png)  
![alt text](image-45.png)  
![alt text](image-46.png)  
![alt text](image-47.png)  
![alt text](image-48.png)  
![alt text](image-49.png)  
![alt text](image-50.png)  
![alt text](image-51.png)  
![alt text](image-52.png)  
и так далее... я больше не можу  
![alt text](image-53.png)  
![alt text](image-54.png)  
  
# Пояснения к реализации
## Классы
Есть два класса Array и List. Имеют идентичные функции, только в Array используется массив, а в List - список.  

## Поля классов
int suffix_count - количество найденный суффиксов (количество листов в дереве + листы могут повторяться, это тоже учитывается)  
int repeat = 0 - количество повторяющихся суффиксов  
  
int branching_count - количество ветвлений (если отрисовать дерево с помощью Print_Tree, то это количество кружочков)  
int internal_nodes_count - количество внутренних вершин (все вершины, не считая корня)  
int total_edges_sum - сумма исходящих ребер из внутренних вершин (все рёбра, не считая рёбра ведущие из корня)  
  
std::string word - данное слово  
int count_leaf = 0 - количество листьев (нужно для вывода дерева с помощью Print_Tree)  
  
## Структуры данных
Есть две структуры: struct Edge - ребро, struct Tree - наше дерево.  
  
Tree содержит:  
- int number_leaf - номер листа  
- Edge* edges[256] - в классе Array для перемещения по листьям  
- Edge* edge_begin - в классе List для перемещения по листьям  

Edge содержит:  
- int start - начальная позиция  
- int end - конечная позиция  
- struct Tree* child - указатель на ребёнка  
- struct Edge* next - в классе List для перемещения по списку  
  
### Ёклмн, что за ребёнок?
Поясню на картинке:  
![alt text](image-55.png)
  
## Описание работы функций
### main()
std::string Generate_Random_String(int length, bool uppercase = false, bool lowercase = true) - функция генерации случайной строки  
void Experiment(std::string wrd) - вызов эксперимента  
void Menu() - менюшка (хороший, плохой случаи, случайные символы, реальный текст и выход)  
int main() - запускает менюшку  
  
### Классы Array и List (конкретно рассмотрены функции класса List)
List(std::string wrd) - конструктор класса, считай main для программы  
int Get_Suffix_Count() - получение поля suffix_count, чтобы убедиться, что программа точно нашла все ожидаемые суффиксы  
  
Tree* Create_Tree(int number_leaf) - создание и инициализация листа  
Edge* Create_Edge(int start, int end, Tree* child = NULL) - создание и инициализация ребра  
void Add_Edge(Tree* node, Edge* edge) - соединение между собой рёбер и листов  
Edge* Find_Edge(Tree* node, char chr) - поиск по первому символу суффикса в дереве  
Tree* Split_Edge(Tree* parent, Edge* edge, int split_pos) - разделение ребра, создание промежуточного узла  
void Insert_Suffix(Tree* root, int start, int end) - добавление листа / суффикса  
  
std::string Get_Edge_String(Edge* edge) - получение строки из начальной и конечной позиции ребра  
void Print_Tree(Tree* node, int depth = 0) - графический вывод дерева (ныне закоменченный, ибо загромождает вывод)  
void Calculate_Statistics(Tree* node, bool is_root = true) - подсчёт branching_count, internal_nodes_count, total_edges_sum  
void Find_Suffixes(Tree* node, std::string suffix_path, std::vector<std::string>& suffixes) - нахождение суффиксов в дереве  
  
void Free_Edge(Edge* edge) - освобождение памяти, занятой рёбрами  
void Free_Tree(Tree* node) - освобождение памяти, занятой листьями  
  