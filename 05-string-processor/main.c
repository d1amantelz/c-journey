#include <stdio.h>

size_t stringLength(const char *str)
{
    const char *p = str;
    while (*p)
    {
        p++;
    }
    return p - str;
}

void reverseString(char *str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    char *left = str;
    char *right = left + stringLength(str) - 1;
    char temp;

    while (left < right)
    {
        temp = *left;
        *left = *right;
        *right = temp;

        left++;
        right--;
    }
}

void trimString(char *str)
{
    char *src = str;

    while (*src == ' ')
    {
        src++;
    }

    char *dst = str;

    while (*src)
    {
        *dst = *src;
        src++;
        dst++;
    }

    *dst = '\0';

    char *end = dst - 1;

    while (end >= str && *end == ' ')
    {
        *end = '\0';
        end--;
    }
}

char *findSubstring(const char *haystack, const char *needle)
{
    const char *h = haystack;

    while (*h)
    {

        const char *h_scout = h;
        const char *n = needle;

        while (*n && *h_scout == *n)
        {
            h_scout++;
            n++;
        }

        if (*n == '\0')
        {
            return (char *)h;
        }

        h++;
    }

    return NULL;
}

void countStats(const char *str, size_t *chars, size_t *words, size_t *lines)
{
    const char *p = str;
    int inWord = 0;

    while (*p)
    {
        (*chars)++;

        if (*p == '\n')
        {
            (*lines)++;
            inWord = 0;
        }
        else if (*p == ' ' || *p == '\t')
        {
            inWord = 0;
        }
        else
        {
            if (!inWord)
            {
                (*words)++;
                inWord = 1;
            }
        }

        p++;
    }

    (*lines)++;
}

int stringEquals(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return 0;

    const char *pstr1 = s1;
    const char *pstr2 = s2;

    while (*pstr1 == *pstr2 && *pstr1 != '\0')
    {
        pstr1++;
        pstr2++;
    }

    if (*pstr1 == *pstr2 && *pstr1 == '\0')
    {
        return 1;
    }

    return 0;
}

void showHelp(char *argv[])
{
    printf("Использование: %s <флаг> \"строка\" [подстрока]\n\n", argv[0]);
    printf("Флаги строкового процессора:\n");
    printf("  --len      <строка>             Измерить длину строки\n");
    printf("  --reverse  <строка>             Развернуть строку на месте\n");
    printf("  --trim     <строка>             Удалить пробелы в начале и конце\n");
    printf("  --find     <текст> <шаблон>     Найти первое вхождение шаблона\n");
    printf("  --stats    <текст>              Подсчитать символы, слова и строки\n");
    printf("  --help                          Показать эту справку\n\n");
    printf("Пример: %s --reverse \"hello world\"\n", argv[0]);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        showHelp(argv);
        return 1;
    }

    if (stringEquals("--help", argv[1]) || stringEquals("-h", argv[1]))
    {
        showHelp(argv);
        return 0;
    }
    else if (stringEquals("--len", argv[1]))
    {
        if (argc < 3)
        {
            printf("Ошибка: для флага --len необходимо передать строку.\n");
            return 1;
        }
        printf("Длина строки: %zu\n", stringLength(argv[2]));
    }
    else if (stringEquals("--reverse", argv[1]))
    {
        if (argc < 3)
        {
            printf("Ошибка: для флага --reverse необходимо передать строку.\n");
            return 1;
        }
        char *tempStr = argv[2];
        reverseString(tempStr);
        printf("Перевернутая строка: %s\n", tempStr);
    }
    else if (stringEquals("--trim", argv[1]))
    {
        if (argc < 3)
        {
            printf("Ошибка: для флага --trim необходимо передать строку.\n");
            return 1;
        }
        char *tempStr = argv[2];
        trimString(tempStr);
        printf("Очищенная строка: %s\n", tempStr);
    }
    else if (stringEquals("--find", argv[1]))
    {
        if (argc < 4)
        {
            printf("Ошибка: для флага --find необходимо передать текст и шаблон.\n");
            return 1;
        }
        char *result = findSubstring(argv[2], argv[3]);

        if (result)
        {
            printf("Первое вхождение шаблона на индексе: %ld\n", result - argv[2]);
        }
        else
        {
            printf("Шаблон в тексте не найден!\n");
        }
    }

    else if (stringEquals("--stats", argv[1]))
    {
        if (argc < 3)
        {
            printf("Ошибка: для флага --stats необходимо передать текст.\n");
            return 1;
        }
        size_t chars = 0;
        size_t words = 0;
        size_t lines = 0;

        countStats(argv[2], &chars, &words, &lines);

        printf("Проанализировав текст была собрана следующая статистика:\n");
        printf(" - Символов в тексте: %zu\n", chars);
        printf(" - Слов в тексте: %zu\n", words);
        printf(" - Строк в тексте: %zu\n", lines);
    }
    else
    {
        printf("Неизвестный флаг: %s\n", argv[1]);
        printf("Используйте %s --help для справки.\n", argv[0]);
        return 1;
    }

    return 0;
}