#include <stdio.h>
#include <string.h>

#define MAX_TASKS 100
#define TASK_LEN 64

void printOptions();
void printTasks(char tasks[][TASK_LEN], int completed[], int task_count);
void addTask(char tasks[][TASK_LEN], int completed[], int *task_count);
void completeTask(int completed[], int task_count);
void deleteTask(char tasks[][TASK_LEN], int completed[], int *task_count);
int getInt();
void getString(char str[], int max_len);

int main() {    
    int choice, task_count;

    char tasks[MAX_TASKS][TASK_LEN];
    int completed[MAX_TASKS];    
    task_count = 0;
    choice = -1;

    while (choice) {
        printOptions();
        choice = getInt();

        switch (choice) {
            case 1: 
                printTasks(tasks, completed, task_count);
                break;
            case 2:
                addTask(tasks, completed, &task_count);
                break;
            case 3:
                completeTask(completed, task_count);
                break;
            case 4:
                deleteTask(tasks, completed, &task_count);
                break;
            case 0:
                printf("Пока!\n");
                return 0;
            default:
                printf("Неверный пункт!\n");
                break;
        }
    }

    return 0;
}

void printOptions() {
    printf("Выбери действие: 1. Список задач, 2. Добавить задачу, 3. Выполнить задачу, 4. Удалить задачу, 0. Выйти\n");
}

int getInt() {
    int value;
    scanf("%d", &value);
    while (getchar() != '\n');
    return value;
}

void getString(char str[], int max_len) {
    fgets(str, max_len, stdin);
    str[strcspn(str, "\n")] = '\0'; 
}

void printTasks(char tasks[][TASK_LEN], int completed[], int task_count) {
    if (task_count == 0) {
        printf("Список задач пока пуст!\n");
        return;
    }

    for (int i = 0; i < task_count; i++) {
        if (completed[i]) {
            printf("%d. [x] %s\n", i + 1, tasks[i]);    
        } else {
            printf("%d. [ ] %s\n", i + 1, tasks[i]);
        }
    }
}

void addTask(char tasks[][TASK_LEN], int completed[], int *task_count) {
    if (*task_count >= MAX_TASKS) {
        printf("Список полон. Невозможно добавить больше задач!\n");
        return;
    }

    printf("Введите задачу: ");
    getString(tasks[*task_count], TASK_LEN);
    
    completed[*task_count] = 0;
    (*task_count)++;
}

void completeTask(int completed[], int task_count) {
    int task_number;
    printf("Номер выполненной задачи: ");
    task_number = getInt();

    if (task_number < 1 || task_number > task_count) {
        printf("Ошибка. Задачи под номером %d не существует!\n", task_number);
        return;
    }
    
    completed[task_number - 1] = 1;
}

void deleteTask(char tasks[][TASK_LEN], int completed[], int *task_count) {
    int index;
    printf("Номер удаляемой задачи: ");
    index = getInt();
    
    if (index < 1 || index > *task_count) {
        printf("Ошибка. Задачи под номером %d не существует!\n", index);
        return;
    }

    for (int i = index - 1; i < *task_count - 1; i++) {
        strcpy(tasks[i], tasks[i + 1]);
        completed[i] = completed[i + 1];
    }

    (*task_count)--;
}
