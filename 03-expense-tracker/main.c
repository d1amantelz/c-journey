#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESCRIPTION 64

typedef enum
{
    FOOD,
    TRANSPORT,
    TECH,
    ENTERTAINMENT,
    OTHER,
    CATEGORY_COUNT
} Category;

typedef struct
{
    int id;
    double amount;
    Category category;
    char description[MAX_DESCRIPTION];
} Expense;

typedef struct
{
    Expense *data;
    int numItems;
    int size;
} list;

void addToList(list *myList, Expense expense);

void printExpenses(list myList);

const char *printCategory(Category category);

void calculateTotal(list myList);

void addExpense(list *myList);

double getDouble();

void getString(char str[], int max_len);

int getInt();

void printOptions();

void printCategoryBreakdown(list myList);

void saveToFile(list myList, FILE *filename);

void importFromFile(list *myList, const char *filename);

int main()
{

    list myList;

    myList.numItems = 0;
    myList.size = 5;
    myList.data = malloc(myList.size * sizeof(Expense));

    if (myList.data == NULL)
    {
        printf("\nВыделение памяти не удалось. Выход из программы!\n");
        return 1;
    }

    importFromFile(&myList, "expenses.csv");

    int choice = -1;

    while (choice)
    {

        printOptions();
        choice = getInt();

        switch (choice)
        {
        case 1:
            addExpense(&myList);
            break;
        case 2:
            printExpenses(myList);
            break;
        case 3:
            calculateTotal(myList);
            break;
        case 4:
            printCategoryBreakdown(myList);
            break;
        case 0:
            printf("\nВыход из программы. До свидания!\n");
            break;
        default:
            printf("\nВыберите правильный пункт меню!\n");
        }
    }

    FILE *file = fopen("expenses.csv", "w");

    if (file != NULL)
    {
        saveToFile(myList, file);
        fclose(file);
    }

    free(myList.data);
    myList.data = NULL;

    return 0;
}

void printOptions()
{
    printf("\n--- Меню ---\n");
    printf("\n1. Добавить расход\n2. Вывести расходы\n3. Посчитать все расходы\n4. Вывести траты по категориям\n0. Выход\n> ");
}

void addToList(list *myList, Expense expense)
{
    if (myList->numItems == myList->size)
    {
        int newSize = myList->numItems + 5;

        Expense *temp = realloc(myList->data, newSize * sizeof(Expense));
        if (temp == NULL)
        {
            printf("\nПеренос памяти не удался!\n");
            return;
        }

        myList->data = temp;
        myList->size = newSize;
    }

    myList->data[myList->numItems] = expense;
    myList->numItems++;
}

const char *printCategory(Category category)
{
    switch (category)
    {
    case FOOD:
        return "Еда";
    case TRANSPORT:
        return "Транспорт";
    case TECH:
        return "Техника";
    case ENTERTAINMENT:
        return "Развлечение";
    case OTHER:
        return "Прочее";
    default:
        return "Неизвестно";
    }
    return NULL;
}

void printExpenses(list myList)
{
    int i;

    printf("\nВсе ваши расходы: \n");
    for (i = 0; i < myList.numItems; i++)
    {
        printf("%d | %.1f | %s | %s\n",
               myList.data[i].id,
               myList.data[i].amount,
               printCategory(myList.data[i].category),
               myList.data[i].description);
    }
}

void calculateTotal(list myList)
{
    int i;
    double total = 0;

    for (i = 0; i < myList.numItems; i++)
    {
        total += myList.data[i].amount;
    }

    printf("\nОбщие расходы составили: %.1f\n", total);
}

int getInt()
{
    int value;
    scanf("%d", &value);
    while (getchar() != '\n')
        ;
    return value;
}

double getDouble()
{
    double value;
    scanf("%lf", &value);
    while (getchar() != '\n')
        ;
    return value;
}

void getString(char str[], int max_len)
{
    fgets(str, max_len, stdin);
    str[strcspn(str, "\n")] = '\0';
}

Category getCategory()
{
    int num;

    while (1)
    {

        printf("\nВыберите нужную категорию:\n");

        for (Category c = 0; c < CATEGORY_COUNT; c++)
        {
            printf("%d. %s\n", c + 1, printCategory(c));
        }

        printf("> ");

        num = getInt();

        if (num >= 1 && num <= CATEGORY_COUNT)
        {
            break;
        }

        printf("\nОшибка! выберите пункт от 1 до %d!\n", CATEGORY_COUNT);
    }

    return (Category)(num - 1);
}

void addExpense(list *myList)
{

    Expense expense;
    expense.id = myList->numItems + 1;

    printf("\nВведите сумму расхода: ");
    expense.amount = getDouble();
    expense.category = getCategory();
    printf("\nВведите описание расхода: ");
    getString(expense.description, MAX_DESCRIPTION);

    addToList(myList, expense);

    printf("\nРасход был успешно добавлен!\n");
}

void printCategoryBreakdown(list myList)
{

    int i, c;
    double totals[CATEGORY_COUNT] = {0};

    for (i = 0; i < myList.numItems; i++)
    {
        totals[myList.data[i].category] += myList.data[i].amount;
    }

    printf("\n-- Ваши расходы по категориям --\n");
    for (c = 0; c < CATEGORY_COUNT; c++)
    {
        printf("* %s - %.2lf руб.\n", printCategory(c), totals[c]);
    }
}

void saveToFile(list myList, FILE *file)
{

    if (file == NULL)
    {
        printf("\nНе удалось сохранить в файл!\n");
        return;
    }

    int i;

    for (i = 0; i < myList.numItems; i++)
    {
        fprintf(file, "%d,%.2f,%d,%s\n",
                myList.data[i].id,
                myList.data[i].amount,
                myList.data[i].category,
                myList.data[i].description);
    }

    printf("\nСохранено в файл!\n");
}

void importFromFile(list *myList, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("\nНе удалось импортировать даннных из файла!\n");
        return;
    }

    char currentString[100];

    while (fgets(currentString, sizeof(currentString), file))
    {
        currentString[strcspn(currentString, "\r\n")] = '\0';

        if (strlen(currentString) == 0)
            continue;

        char *idStr = strtok(currentString, ",");
        char *amStr = strtok(NULL, ",");
        char *catStr = strtok(NULL, ",");
        char *descStr = strtok(NULL, ",");

        if (idStr && amStr && catStr && descStr)
        {
            Expense expense;
            expense.id = atoi(idStr);
            expense.amount = atof(amStr);
            expense.category = (Category)atoi(catStr);

            strncpy(expense.description, descStr, MAX_DESCRIPTION - 1);
            expense.description[MAX_DESCRIPTION - 1] = '\0';

            addToList(myList, expense);
        }
    }

    fclose(file);
    printf("\nЗагружено расходов из файла: %d\n", myList->numItems);
}