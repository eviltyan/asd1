#include "Array.h"
#include "List.h"

int main()
{
    std::string word = "ababa";
    std::cout << "Строка: \"" << word << "\"" << std::endl;

    std::cout << std::endl << "ОЖИДАЕМЫЕ СУФФИКСЫ" << std::endl;
    for (int i = 0; i < word.length(); i++)
    {
        std::string suffix;
        for (int j = i; j < word.length(); j++)
        {
            suffix += word[j];
        }
        std::cout << "\"" << suffix << "\"" << std::endl;
    }

	std::cout << std::endl << "===== МАССИВ =====" << std::endl;
	Array array(word);

	std::cout << std::endl << "===== СПИСОК =====" << std::endl;
	List list(word);

	std::string end_program;
	std::cin >> end_program;
}