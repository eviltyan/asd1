#include <random>
#include <ctime>
#include "Array.h"
#include "List.h"

std::string Generate_Random_String(int length, bool uppercase = false, bool lowercase = true)
{
    std::string chars;
    if (uppercase) chars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (lowercase) chars += "abcdefghijklmnopqrstuvwxyz";

    if (chars.empty()) chars = "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.length() - 1);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; i++)
    {
        result += chars[dis(gen)];
    }

    return result;
}

void Experiment(std::string wrd)
{
    std::string word = wrd;
    std::cout << "СТРОКА: \"" << word << "\"" << std::endl;

    std::cout << std::endl << "ОЖИДАЕМЫЕ СУФФИКСЫ" << std::endl;

    int expected_count = 0;
    for (int i = 0; i < word.length(); i++)
    {
        std::string suffix;
        for (int j = i; j < word.length(); j++)
        {
            suffix += word[j];
        }
        std::cout << "\"" << suffix << "\"" << std::endl;
        expected_count++;
    }

    std::cout << std::endl << "===== МАССИВ =====" << std::endl;
    Array array(word);

    std::cout << std::endl << "===== СПИСОК =====" << std::endl;
    List list(word);

    if (array.Get_Suffix_Count() == expected_count)
    {
        std::cout << std::endl  << "НАЙДЕНЫ ВСЕ СУФФИКСЫ (В КОЛИЧЕСТВЕ " 
                                << expected_count 
                                << ") С ПОМОЩЬЮ МАССИВА!";
    }
        

    if (list.Get_Suffix_Count() == expected_count)
    {
        std::cout << std::endl  << "НАЙДЕНЫ ВСЕ СУФФИКСЫ (В КОЛИЧЕСТВЕ "
                                << expected_count
                                << ") С ПОМОЩЬЮ СПИСКА!" << std::endl;
    }
    else
    {
        std::cout << list.Get_Suffix_Count() << " " << expected_count << std::endl;
    }
        

    std::cout << std::endl;
}

void Menu()
{
    std::cout << "Ваш выбор: ";

    int choice = 0;
    std::cin >> choice;

    std::string text_good;
    std::string text_bad;
    std::string text_real;

    switch (choice)
    {
    case 1:
        std::cout << "------- ЭКСПЕРИМЕНТ (ХОРОШИЙ СЛУЧАЙ) -------" << std::endl;
        text_good =
            "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqr"
            "stuvwxyzabcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba";

        Experiment(text_good);
        Menu();
        break;

    case 2:
        std::cout << "------- ЭКСПЕРИМЕНТ (ПЛОХОЙ СЛУЧАЙ) -------" << std::endl;
        text_bad =
            "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
            "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
            "fffffffffffffffffffffffffffffffffffffi";

        Experiment(text_bad);
        Menu();
        break;

    case 3:
        std::cout << "------- ЭКСПЕРИМЕНТ (СЛУЧАЙНЫЙ НАБОР СИМВОЛОВ) -------" << std::endl;
        Experiment(Generate_Random_String(100));
        Menu();
        break;

    case 4:
        std::cout << "------- ЭКСПЕРИМЕНТ (РЕАЛЬНЫЙ ТЕКСТ) -------" << std::endl;
        text_real =
            "It was the best of times it was the worst of times it was the age of wisdom "
            "it was the age of foolishness it was the epoch of belief it was the epoch of "
            "incredulity it was the season of light it was the season of darkness it was "
            "the spring of hope it was the winter of despair we had everything before us "
            "we had nothing before us we were all going direct to heaven we were all going "
            "direct the other way in short the period was so far like the present period "
            "that some of its noisiest authorities insisted on its being received for good "
            "or for evil in the superlative degree of comparison only";

        Experiment(text_real);
        Menu();
        break;

    case 5:
        exit(0);

    default:
        std::cout << "Некорректный ввод!" << std::endl;
        Menu();
    }
}

int main()
{
    std::cout
        << "Доступные функции:" << std::endl
        << "1) Хороший случай" << std::endl
        << "2) Плохой случай" << std::endl
        << "3) Полностью случайный набор символов" << std::endl
        << "4) Реальный текст" << std::endl
        << "5) Выход из программы" << std::endl << std::endl;

    Menu();

    std::string end_program;
    std::cin >> end_program;
    return 0;
}