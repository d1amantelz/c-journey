#include <stdio.h>
#include <string.h>

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char note[100];
} Contact;

int getInt();
void getString(char str[], int max_len);
void printOptions();
void showContacts(Contact contacts[MAX_CONTACTS], int contacts_count);
void addContact(Contact contacts[MAX_CONTACTS], int *contacts_count);
void findContact(Contact contacts[MAX_CONTACTS], int contacts_count);
void deleteContact(Contact contacts[MAX_CONTACTS], int *contacts_count);

int main() {
    Contact contacts[MAX_CONTACTS];
    int choice, contacts_count;
    choice = -1;
    contacts_count = 0;

    while (choice) {
        printOptions();
        choice = getInt();

        switch (choice) {
            case 1:
                showContacts(contacts, contacts_count);
                break;
            case 2:
                addContact(contacts, &contacts_count);
                break;
            case 3:
                findContact(contacts, contacts_count);
                break;
            case 4:
                deleteContact(contacts, &contacts_count);
                break;
            case 0:
                printf("\nПока!\n");
                return 0;
            default:
                printf("\nПопробуй снова!\n");
                break;
        }
    }

    return 0;
}

void printOptions() {
    printf("\n --- Меню --- \nВыберите опцию: \n1. Показать все контакты\n2. Добавить контакт\n3. Найти контакт\n4. Удалить контакт\n0. Выход.\n> ");
}

void addContact(Contact contacts[MAX_CONTACTS], int *contacts_count) {
    if (*contacts_count >= MAX_CONTACTS) {
        printf("\nТелефонная книга переполнена. Удалите лишние контакты опцией 4!\n");
        return;
    }

    Contact contact;

    printf("\nВведите имя для нового контакта: ");
    getString(contact.name, 50);

    printf("Введите номер телефона для нового контакта: ");
    getString(contact.phone, 20);

    printf("Введите заметку для нового контакта: ");
    getString(contact.note, 100);

    contacts[*contacts_count] = contact;
    (*contacts_count)++;

    printf("\nКонтакт успешно добавлен!\n");
}

void showContacts(Contact contacts[MAX_CONTACTS], int contacts_count) {
    if (contacts_count == 0) {
        printf("\nСписок контактов пуст. Добавьте контакты опцией 2!\n");
        return;
    }

    int i;
    printf("\n");

    for (i = 0; i < contacts_count; i++) {
        printf("%d | %s | %s | %s\n", i + 1, contacts[i].name, contacts[i].phone, contacts[i].note);
    }
}

void findContact(Contact contacts[MAX_CONTACTS], int contacts_count) {
    if (contacts_count == 0) {
        printf("\nТелефонная книга пуста. Добавьте контакты опцией 2!\n");
        return;
    }

    int i;
    int found = 0;
    char *ptr1, *ptr2, *ptr3;
    char substring[50];
    printf("\nВведите строку для поиска: ");
    getString(substring, 50);

    for (i = 0; i < contacts_count; i++) {
        ptr1 = strstr(contacts[i].name, substring);
        ptr2 = strstr(contacts[i].phone, substring);
        ptr3 = strstr(contacts[i].note, substring);

        if (ptr1 != NULL || ptr2 != NULL || ptr3 != NULL) {
            printf("\n%d | %s | %s | %s", i + 1, contacts[i].name, contacts[i].phone, contacts[i].note);
            found++;
        }
    }

    if (found == 0) {
        printf("\nКонтактов по запросу '%s' не найдено!\n", substring);
    }
}

void deleteContact(Contact contacts[MAX_CONTACTS], int *contacts_count) {
    int index, i;
    printf("\nВведите номер контакта для удаления: ");
    index = getInt();

    if (index < 1 || index > *contacts_count) {
        printf("\nНедопустимый номер контакта!\n");
        return;
    }

    for (i = index - 1; i < *contacts_count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }

    (*contacts_count)--;

    printf("\nКонтакт был успешно удален!\n");
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
