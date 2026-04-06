using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace SubstringSearchAlgorithms
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;
            var analyzer = new SubstringSearchAnalyzer();
            analyzer.RunExperiments();

            Console.WriteLine("\nНажмите любую клавишу для выхода...");
            Console.ReadKey();
        }
    }

    public class SubstringSearchAnalyzer
    {
        private readonly Random _random = new Random(42); // Фиксированное зерно для воспроизводимости

        public void RunExperiments()
        {
            var results = new List<ExperimentResult>();

            results.Add(RunExperiment(
                "Худший случай (все 'a')",
                GenerateString('a', 100000),
                GenerateString('a', 1000)));

            results.Add(RunExperiment(
                "Лучший случай (разные символы)",
                GenerateString('a', 100000),
                GenerateString('b', 1000)));

            results.Add(RunExperiment(
                "Большой алфавит (ASCII 32-126)",
                GenerateRandomString(100000, 32, 126),
                GenerateRandomString(1000, 32, 126)));

            results.Add(RunExperiment(
                "Маленький алфавит (5 букв: A-E)",
                GenerateRandomString(100000, 'A', 'E'),
                GenerateRandomString(1000, 'A', 'E')));

            try
            {
                string realText = GetTextFromFile("text.txt");
                string realWord = "and";

                Console.WriteLine("\nВведите слово для поиска в реальном тексте (по умолчанию 'and'):");
                string input = Console.ReadLine();
                if (!string.IsNullOrWhiteSpace(input))
                {
                    realWord = input;
                }

                results.Add(RunExperiment(
                    $"Реальный текст (файл) + слово '{realWord}'",
                    realText,
                    realWord));
            }
            catch (Exception ex)
            {
                Console.WriteLine($"\nОшибка при чтении файла: {ex.Message}");
                Console.WriteLine("Пропускаем эксперимент с реальным текстом...");
            }

            // Дополнительный эксперимент для демонстрации O(n*m)
            results.AddRange(DemonstrateComplexity());


            PrintResults(results);
        }


        private string GetTextFromFile(string filename)
        {
            if (!File.Exists(filename))
            {
                CreateSampleTextFile(filename);
                Console.WriteLine($"Создан файл-образец: {filename}");
            }

            string text = File.ReadAllText(filename, Encoding.UTF8);

            Console.WriteLine($"Загружено {text.Length} символов из файла {filename}");
            return text;
        }


        private void CreateSampleTextFile(string filename)
        {
            string sampleText = @"It is a truth universally acknowledged, that a single man in possession of a good fortune, 
must be in want of a wife. However little known the feelings or views of such a man may be on his 
first entering a neighbourhood, this truth is so well fixed in the minds of the surrounding families, 
that he is considered the rightful property of some one or other of their daughters. 

My dear Mr. Bennet, said his lady to him one day, have you heard that Netherfield Park is let at last? 
Mr. Bennet replied that he had not. But it is, returned she; for Mrs. Long has just been here, and she told me all about it. 
Mr. Bennet made no answer. Do you not want to know who has taken it? cried his wife impatiently. 
You want to tell me, and I have no objection to hearing it. This was invitation enough. 

Why, my dear, you must know, Mrs. Long says that Netherfield is taken by a young man of large fortune 
from the north of England; that he came down on Monday in a chaise and four to see the place, and was so much delighted with it, 
that he agreed with Mr. Morris immediately; that he is to take possession before Michaelmas, and some of his servants are to be 
in the house by the end of next week. 

What is his name? Bingley. Is he married or single? Oh! Single, my dear, to be sure! 
A single man of large fortune; four or five thousand a year. What a fine thing for our girls! 
How so? How can it affect them? My dear Mr. Bennet, replied his wife, how can you be so tiresome! 
You must know that I am thinking of his marrying one of them. Is that his design in settling here? 
Design! nonsense, how can you talk so! But it is very likely that he may fall in love with one of them, 
and therefore you must visit him as soon as he comes. 

I see no occasion for that. You and the girls may go, or you may send them by themselves, 
which perhaps will be still better, for as you are as handsome as any of them, Mr. Bingley may like you the best of the party. 
My dear, you flatter me. I certainly have had my share of beauty, but I do not pretend to be anything extraordinary now. 
When a woman has five grown-up daughters, she ought to give over thinking of her own beauty. 
In such cases, a woman has not often much beauty to think of. 

But, my dear, you must indeed go and see Mr. Bingley when he comes. 
It is more than I engage for, I assure you. But consider your daughters. 
Only think what an establishment it would be for one of them. 
Sir William and Lady Lucas are determined to go, merely on that account, for in general you know they visit no newcomers. 
Indeed you must go, for it will be impossible for us to visit him if you do not. 

You are overscrupulous, surely. I dare say Mr. Bingley will be very glad to see you; 
and I will send a few lines by you to assure him of my hearty consent to his marrying whichever he chooses of the girls; 
though I must throw in a good word for my little Lizzy. 

I desire you will do no such thing. Lizzy is not a bit better than the others; 
and I am sure she is not half so handsome as Jane, nor half so good-humoured as Lydia. 
But you are always giving her the preference. They have none of them much to recommend them, replied he; 
they are all silly and ignorant like other girls; but Lizzy has something more of quickness than her sisters. 

Mr. Bennet, how can you abuse your own children in such a way? You take delight in vexing me. 
You have no compassion for my poor nerves. You mistake me, my dear. 
I have a high respect for your nerves. They are my old friends. 
I have heard you mention them with consideration these last twenty years at least. 

Mr. Bennet, you do not make yourself agreeable to me. I beg you will not dwell upon my nerves. 
Oh! Mr. Bennet, you are wanted; and immediately. We are all in an uproar. 
You must come and make Lizzy marry Mr. Collins, for she vows she will not have him, 
and if you do not make haste he will change his mind and not have her.

This is a sample text from Jane Austen's ""Pride and Prejudice"". 
You can replace this file with your own text file named 'text.txt' 
to test the algorithms on different real texts.

The quick brown fox jumps over the lazy dog. 
Pack my box with five dozen liquor jugs. 
How vexingly quick daft zebras jump!";

            File.WriteAllText(filename, sampleText, Encoding.UTF8);
        }

        private List<ExperimentResult> DemonstrateComplexity()
        {
            var results = new List<ExperimentResult>();
            var pattern = GenerateString('a', 100); // Фиксированная подстрока

            Console.WriteLine("\n=== Демонстрация сложности O(n*m) ===");

            for (int n = 1000; n <= 10000; n += 1000)
            {
                var text = GenerateString('a', n); // Худший случай
                var result = RunExperiment($"Сложность O(n*m) n={n}", text, pattern);
                results.Add(result);

                if (n == 10000)
                {
                    pattern = GenerateString('a', 10);
                    result = RunExperiment($"Сложность O(n*m) n={n}", text, pattern);
                    results.Add(result);
                    pattern = GenerateString('a', 50);
                    result = RunExperiment($"Сложность O(n*m) n={n}", text, pattern);
                    results.Add(result);
                    pattern = GenerateString('a', 250);
                    result = RunExperiment($"Сложность O(n*m) n={n}", text, pattern);
                    results.Add(result);
                    pattern = GenerateString('a', 500);
                    result = RunExperiment($"Сложность O(n*m) n={n}", text, pattern);
                    results.Add(result);
                    pattern = GenerateString('a', 1000);
                    result = RunExperiment($"Сложность O(n*m) n={n}", text, pattern);
                    results.Add(result);
                }

                Console.WriteLine($"n={n}, время наивного: {result.NaiveTimeMs:F2} мс, " +
                                $"отношение к n*m: {result.NaiveTimeMs / (n * pattern.Length / 1000000.0):F2}");
            }
            return results;
        }


        private ExperimentResult RunExperiment(string name, string text, string pattern)
        {
            Console.WriteLine($"\nВыполняется эксперимент: {name}");

            var naive = new NaiveSubstringSearch();
            var kmp = new KMPSubstringSearch();
            var rabinKarp = new RabinKarpSubstringSearch();

            // Разогрев JIT
            WarmUp(naive, kmp, rabinKarp, text, pattern);

            // Результаты
            List<int> naiveResult = null;
            List<int> kmpResult = null;
            List<int> rabinKarpResult = null;

            // Измерение времени + запись результатов
            int iter = text.Length > 50000 ? 3 : 10;
            var naiveTime = MeasureTime(() => {
                naiveResult = naive.Search(text, pattern);
                return naiveResult;
            }, iter);
            var kmpTime = MeasureTime(() => { 
                kmpResult = kmp.Search(text, pattern); 
                return kmpResult;
            }, iter);
            var rabinKarpTime = MeasureTime(() => {
                rabinKarpResult = rabinKarp.Search(text, pattern);
                return rabinKarpResult;
            }, iter);

            
            // Проверка корректности
            if (!naiveResult.SequenceEqual(kmpResult) || !naiveResult.SequenceEqual(rabinKarpResult))
            {
                Console.WriteLine("ВНИМАНИЕ: Результаты алгоритмов не совпадают!");
            }

            // Анализ префиксов-суффиксов
            var prefixSuffixStats = AnalyzePrefixSuffix(text, pattern);

            return new ExperimentResult
            {
                Name = name,
                TextLength = text.Length,
                PatternLength = pattern.Length,
                NaiveTimeMs = naiveTime,
                KMPTimeMs = kmpTime,
                RabinKarpTimeMs = rabinKarpTime,
                PatternOccurrences = naiveResult.Count,
                CommonPrefixesCount = prefixSuffixStats.CommonPrefixesCount,
                AverageCommonPrefixLength = prefixSuffixStats.AverageCommonPrefixLength,
                TotalCommonPrefixLength = prefixSuffixStats.TotalCommonPrefixLength,
                TextSample = text.Length > 50 ? text.Substring(0, 50) + "..." : text,
                PatternSample = pattern.Length > 30 ? pattern.Substring(0, 30) + "..." : pattern
            };
        }

        private PrefixSuffixStats AnalyzePrefixSuffix(string text, string pattern)
        {
            int count = 0;
            int totalLength = 0;
            int maxLength = Math.Min(pattern.Length, text.Length);

            for (int i = 1; i <= maxLength; i++) // Каждый раз создавалась новая строка и получалось O(n²) - это плохо
            {
                bool match = true;
                for (int j = 0; j < i; j++)
                {
                    if (text[text.Length - i + j] != pattern[j])
                    {
                        match = false;
                        break;
                    }
                }

                if (match)
                {
                    count++;
                    totalLength += i;
                }
            }

            return new PrefixSuffixStats
            {
                CommonPrefixesCount = count,
                TotalCommonPrefixLength = totalLength,
                AverageCommonPrefixLength = count > 0 ? (double)totalLength / count : 0
            };
        }

        private void WarmUp(ISubstringSearch naive, ISubstringSearch kmp, ISubstringSearch rabinKarp,
                            string text, string pattern)
        {
            naive.Search(text, pattern).ToList();
            kmp.Search(text, pattern).ToList();
            rabinKarp.Search(text, pattern).ToList();
        }

        private double MeasureTime(Func<List<int>> action, int iterations = 5)
        {
            double total = 0;

            for (int i = 0; i < iterations; i++)
            {
                var sw = Stopwatch.StartNew();
                action();
                sw.Stop();
                total += sw.Elapsed.TotalMilliseconds;
            }
            return total / iterations;
        }

        private string GenerateString(char c, int length)
        {
            return new string(c, length);
        }

        private string GenerateRandomString(int length, char minChar, char maxChar)
        {
            var sb = new StringBuilder(length);
            int range = maxChar - minChar + 1;

            for (int i = 0; i < length; i++)
            {
                sb.Append((char)(minChar + _random.Next(range)));
            }

            return sb.ToString();
        }

        private string GenerateRandomString(int length, int minCode, int maxCode)
        {
            var sb = new StringBuilder(length);
            int range = maxCode - minCode + 1;

            for (int i = 0; i < length; i++)
            {
                sb.Append((char)(minCode + _random.Next(range)));
            }

            return sb.ToString();
        }

        private void PrintResults(List<ExperimentResult> results)
        {
            Console.WriteLine("\nРЕЗУЛЬТАТЫ ЭКСПЕРИМЕНТОВ\n");

            Console.WriteLine(new string('═', 130));
            Console.WriteLine($"║ {"Эксперимент",-30} ║ {"n",6} ║ {"m",6} ║ {"Naive",10} ║ {"KMP",10} ║ {"RK",10} ║ {"ускор.",10} ║");
            Console.WriteLine(new string('═', 130));

            foreach (var r in results)
            {
                double speedup = r.KMPTimeMs > 0 ? r.NaiveTimeMs / r.KMPTimeMs : 0;

                Console.WriteLine(
                    $"║ {Trim(r.Name, 30),-30} ║ {r.TextLength,6} ║ {r.PatternLength,6} ║ {r.NaiveTimeMs,10:F2} ║ {r.KMPTimeMs,10:F2} ║ {r.RabinKarpTimeMs,10:F2} ║ {speedup,10:F2} ║"
                );
            }

            Console.WriteLine(new string('═', 130));

            Console.WriteLine("\nАНАЛИЗ:");
            foreach (var r in results)
            {
                Console.WriteLine($"\n{r.Name}:");
                Console.WriteLine($"  Naive/KMP = {(r.NaiveTimeMs / r.KMPTimeMs):F2}");
                Console.WriteLine($"  Naive/RK  = {(r.NaiveTimeMs / r.RabinKarpTimeMs):F2}");
            }

        }
        private string Trim(string text, int maxLength)
        {
            return text.Length > maxLength
                ? text.Substring(0, maxLength - 3) + "..."
                : text;
        }
    }
    

    // Интерфейс для алгоритмов поиска подстроки
    public interface ISubstringSearch
    {
        List<int> Search(string text, string pattern);
    }


    public class NaiveSubstringSearch : ISubstringSearch
    {
        public List<int> Search(string text, string pattern)
        {
            var result = new List<int>();
            int n = text.Length;
            int m = pattern.Length;

            if (m > n || m == 0) return result;

            for (int i = 0; i <= n - m; i++)
            {
                int j = 0;
                while (j < m && text[i + j] == pattern[j])
                {
                    j++;
                }
                if (j == m)
                {
                    result.Add(i);
                }
            }

            return result;
        }
    }


    public class KMPSubstringSearch : ISubstringSearch
    {
        public List<int> Search(string text, string pattern)
        {
            var result = new List<int>();
            int n = text.Length;
            int m = pattern.Length;

            if (m > n || m == 0) return result;

            int[] lps = ComputeLPS(pattern);

            int i = 0; // индекс для text
            int j = 0; // индекс для pattern

            while (i < n)
            {
                if (pattern[j] == text[i])
                {
                    i++;
                    j++;
                }

                if (j == m)
                {
                    result.Add(i - j);
                    j = lps[j - 1];
                }
                else if (i < n && pattern[j] != text[i])
                {
                    if (j != 0)
                    {
                        j = lps[j - 1];
                    }
                    else
                    {
                        i++;
                    }
                }
            }
            return result;
        }

        private int[] ComputeLPS(string pattern)
        {
            int m = pattern.Length;
            int[] lps = new int[m];
            int length = 0;
            int i = 1;

            lps[0] = 0;

            while (i < m)
            {
                if (pattern[i] == pattern[length])
                {
                    length++;
                    lps[i] = length;
                    i++;
                }
                else
                {
                    if (length != 0)
                    {
                        length = lps[length - 1];
                    }
                    else
                    {
                        lps[i] = 0;
                        i++;
                    }
                }
            }
            return lps;
        }
    }


    public class RabinKarpSubstringSearch : ISubstringSearch
    {
        private const int Base = 256; // Размер алфавита
        private const int Prime = 1_000_000_007; //было 101 // Простое число для хэширования

        public List<int> Search(string text, string pattern)
        {
            var result = new List<int>();
            int n = text.Length;
            int m = pattern.Length;

            if (m > n || m == 0) return result;

            // Вычисляем хэш для паттерна и первого окна текста
            long patternHash = 0;
            long textHash = 0;
            long h = 1;

            // Значение h = pow(Base, m-1) % Prime
            for (int i = 0; i < m - 1; i++)
            {
                h = (h * Base) % Prime;
            }

            // Вычисляем начальные хэши
            for (int i = 0; i < m; i++)
            {
                patternHash = (Base * patternHash + pattern[i]) % Prime;
                textHash = (Base * textHash + text[i]) % Prime;
            }

            // Сканер текста
            for (int i = 0; i <= n - m; i++)
            {
                // Проверка посимвольно если хэши совпадают
                if (patternHash == textHash)
                {
                    int j;
                    for (j = 0; j < m; j++)
                    {
                        if (text[i + j] != pattern[j])
                            break;
                    }
                    if (j == m)
                        result.Add(i);
                }

                // Вычисляем хэш для следующего окна
                if (i < n - m)
                {
                    textHash = (Base * (textHash - text[i] * h) + text[i + m]) % Prime;

                    // Если хэш стал отрицательным, делаем его положительным
                    if (textHash < 0)
                        textHash += Prime;
                }
            }
            return result;
        }
    }


    // Класс для хранения статистики о префиксах-суффиксах
    public class PrefixSuffixStats
    {
        public int CommonPrefixesCount { get; set; }
        public int TotalCommonPrefixLength { get; set; }
        public double AverageCommonPrefixLength { get; set; }
    }


    // Класс для хранения результатов эксперимента
    public class ExperimentResult
    {
        public string Name { get; set; }
        public int TextLength { get; set; }
        public int PatternLength { get; set; }
        public double NaiveTimeMs { get; set; }
        public double KMPTimeMs { get; set; }
        public double RabinKarpTimeMs { get; set; }
        public int PatternOccurrences { get; set; }
        public int CommonPrefixesCount { get; set; }
        public double AverageCommonPrefixLength { get; set; }
        public int TotalCommonPrefixLength { get; set; }
        public string TextSample { get; set; }
        public string PatternSample { get; set; }
    }
}