#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char **lines;
    size_t count;
    size_t capacity;
} StringArray;

int initArray(StringArray *arr);

void appendLine(StringArray *arr, const char *str);

void printArray(const StringArray *arr);

int removeLine(StringArray *arr, size_t index);

int freeArray(StringArray *arr);

void sortLines(StringArray *arr);

int main()
{
    StringArray array;

    if (!initArray(&array))
    {
        printf("\nНе удалось инициализировать динамический массив. Выход из программы!\n");
        return 1;
    }

    appendLine(&array, "Delta");
    appendLine(&array, "Alpha");
    appendLine(&array, "Echo");
    appendLine(&array, "Bravo");
    appendLine(&array, "Charlie");

    printf("\n--- Исходный массив ---");
    printArray(&array);

    sortLines(&array);
    printf("\n--- После сортировки (O(1) pointer swap) ---");
    printArray(&array);

    removeLine(&array, 2); // Удаляем "Charlie"
    printf("\n--- После удаления элемента с индексом 2 ---");
    printArray(&array);

    if (!freeArray(&array))
    {
        printf("\nНе удалось освободить массив из памяти!\n");
    }

    return 0;
}

int initArray(StringArray *arr)
{
    arr->count = 0;
    arr->capacity = 4;
    arr->lines = malloc(arr->capacity * sizeof(char *));

    if (!arr->lines)
    {
        printf("\nНе удалось выделить память. Выход из программы!\n");
        return 0;
    }
    return 1;
}

void appendLine(StringArray *arr, const char *str)
{
    if (arr->count == arr->capacity)
    {
        int newCapacity = arr->capacity * 2;
        char **temp = realloc(arr->lines, newCapacity * sizeof(char *));

        if (temp == NULL)
        {
            printf("\nПеренос памяти не удался!\n");
            return;
        }

        arr->lines = temp;
        arr->capacity = newCapacity;
    }

    size_t strLength = strlen(str);
    char *newLine = malloc(strLength * sizeof(char) + 1);

    if (newLine == NULL)
    {
        printf("\nНе удалось зарезервировать память!\n");
        return;
    }

    for (size_t i = 0; i <= strLength; i++)
    {
        *(newLine + i) = str[i];
    }

    arr->lines[arr->count] = newLine;
    arr->count++;
}

void printArray(const StringArray *arr)
{
    char **start = arr->lines;
    char **end = arr->lines + arr->count;
    int count = 0;
    printf("\nStringArray[]:\n");

    while (start < end)
    {
        printf("  %d. %s\n", count, *start);
        start++;
        count++;
    }
}

int removeLine(StringArray *arr, size_t index)
{

    if (index >= arr->count)
    {
        printf("\nОшибка: индекс %zu выходит за пределы массива!\n", index);
        return 0;
    }

    char *delString = arr->lines[index];
    free(delString);

    for (size_t i = index; i < arr->count - 1; i++)
    {
        arr->lines[i] = arr->lines[i + 1];
    }

    arr->count--;
    return 1;
}

int freeArray(StringArray *arr)
{
    for (size_t i = 0; i < arr->count; i++)
    {
        free(arr->lines[i]);
    }

    free(arr->lines);
    arr->lines = NULL;
    arr->capacity = 0;
    arr->count = 0;
    return 1;
}

void sortLines(StringArray *arr)
{
    if (arr->count < 2)
    {
        return;
    }

    for (size_t i = 0; i < arr->count - 1; i++)
    {
        for (size_t j = 0; j < arr->count - 1 - i; j++)
        {
            if (strcmp(arr->lines[j], arr->lines[j + 1]) > 0)
            {
                char *temp = arr->lines[j + 1];
                arr->lines[j + 1] = arr->lines[j];
                arr->lines[j] = temp;
            }
        }
    }
}