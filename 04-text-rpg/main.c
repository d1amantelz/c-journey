#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH_NAME 32

typedef struct
{
    int id;
    char name[MAX_LENGTH_NAME];
    int attack;
    int hp;
    int cost;
} Item;

typedef struct
{
    Item *data;
    int numItems;
    int size;
} Inventory;

typedef struct
{
    char name[MAX_LENGTH_NAME];
    Inventory *inventory;
    int hp;
    int attack;
    int gold;
} Player;

void addToInventory(Inventory *inventory, Item item);

void showInventory(const Inventory *inventory);

int main()
{
    Item item1 = {1, "Ржавый кинжал", 1, 0, 3};
    Item item2 = {2, "Хлеб", 0, 3, 2};
    Item item3 = {3, "Водка", 1, -1, 4};

    Inventory inventory;
    inventory.numItems = 0;
    inventory.size = 10;
    inventory.data = malloc(inventory.size * sizeof(Item));

    if (inventory.data == NULL)
    {
        printf("\nВыделение памяти не удалось. Выход из игры!\n");
        return 1;
    }

    addToInventory(&inventory, item1);
    addToInventory(&inventory, item2);
    addToInventory(&inventory, item3);

    showInventory(&inventory);

    free(inventory.data);
    return 0;
}

void addToInventory(Inventory *inventory, Item item)
{
    if (inventory->numItems == inventory->size)
    {
        int newSize = inventory->numItems + 5;

        Item *temp = realloc(inventory->data, newSize * sizeof(Item));
        if (temp == NULL)
        {
            printf("\nПеренос памяти не удался!\n");
            return;
        }

        inventory->data = temp;
        inventory->size = newSize;
    }

    inventory->data[inventory->numItems] = item;
    inventory->numItems++;
}

void showInventory(const Inventory *inventory)
{
    printf("\nИнвентарь (%d/%d):\n",
           inventory->numItems, inventory->size);

    for (int i = 0; i < inventory->numItems; i++)
    {
        const Item *item = &inventory->data[i];

        char bonuses[64] = "";
        int hasHp = item->hp != 0;
        int hasAtk = item->attack != 0;

        if (hasHp && hasAtk)
        {
            snprintf(bonuses, sizeof bonuses,
                     "%+d HP | %+d ATK", item->hp, item->attack);
        }
        else if (hasHp)
        {
            snprintf(bonuses, sizeof bonuses, "%+d HP", item->hp);
        }
        else if (hasAtk)
        {
            snprintf(bonuses, sizeof bonuses, "%+d ATK", item->attack);
        }

        if (bonuses[0] != '\0')
        {
            printf("  %d. %s (%s)\n", i + 1, item->name, bonuses);
        }
        else
        {
            printf("  %d. %s\n", i + 1, item->name);
        }
    }
}

/*
Главный герой
Имя name строка
Инвентарь inventory массив из предметов
Здоровье hp int
Атака attack int
Золото gold int

Инвентарь
data где хранится массив из структур Предмет
size вместимость инвентаря (10 дефолт)
numItems кол-во предметов в инвентаре

Предмет
id int
name строка
attack int (если не дает атаку, то 0)
hp int (если не дает hp, то 0)
cost int цена

Локация
name название локации
intro строка с описанием при входе на локацию
options набор опций выбора следующего действия
neighbours соседи локации, куда можно попасть дальше


*/

/*

==================================================
              ОТБЛЕСК ЛУННОГО СВЕТА
                      v0.1
==================================================

Введите имя героя: > Ебло

Добро пожаловать, Ебло!
Ты очнулся на берегу синего моря.
Теплая волна омывает твои ноги.
Ты медленно встаешь и рассматриваешь

место вокруг...


--------------------------------------------------
Берег синего моря
HP: 20/20   ATK: 3   GOLD: 10
--------------------------------------------------

Вы видите бескрайний океан, который
освещается яркой полной луной.
На поясе у вас висит - ржавый кинжал.
В стороне вы видите дым из труб,
а также огонь костров. Видимо
это Деревня...

1. Идти в Деревня Угли
2. Инвентарь
> 2

Инвентарь (3/10):
  1. Ржавый кинжал  (+1 ATK)
  2. Хлеб           (+3 HP)
  3. Водка          (-1 HP | +1 ATK)

1. Использовать
2. Выбросить
3. Назад
> 1

1. Ржавый кинжал
2. Хлеб
3. Водка
4. Назад
> 1

Вы взяли в руку ржавый кинжал

1. Ржавый кинжал
2. Хлеб
3. Водка
4. Назад
> 4

Инвентарь (3/10):
  1. Хлеб           (+3 HP)
  2. Водка          (-1 HP | +1 ATK)

1. Использовать
2. Выбросить
3. Назад
> 3

1. Идти в Деревня Угли
2. Инвентарь
> 1


--------------------------------------------------
Деревня Угли
HP: 20/20   ATK: 4   GOLD: 10
--------------------------------------------------

В деревне вы видите много разных людей,
начиная от детей заканчивая стариками,
сидящими молча возле главного центрального
костра...

1. Идти в Лавка загадочного торговца
2. Идти к Берег синего моря
3. Инвентарь
> 1

--------------------------------------------------
Лавка загадочного торговца
HP: 20/20   ATK: 4   GOLD: 10
--------------------------------------------------

Зайдя в лавку, первое что вы увидели,
это загадочный мужчина в капюшоне,
который занимался игрой в шахматы сам
с собой...

- Здравствуй, незнакомый человек,
  вижу ты у меня впервые. Ты хочешь
  совершить покупку или попросить
  совета?

1. Зелье лечения        (+10 HP) - 8 GOLD
2. Серебряный кинжал    (+5 ATK) - 10 GOLD
3. Попросить совета
4. Уйти
> 3

- Знаешь, путник. Не советовал бы я
  тебе находиться здесь днем. Мало ли
  еще не знаешь. А так хоть живым останешься...

1. Зелье лечения        (+10 HP) - 8 GOLD
2. Серебряный кинжал    (+5 ATK) - 10 GOLD
3. Попросить совета
4. Уйти
> 1

Куплено: Зелье лечения. Осталось GOLD: 2.

1. Зелье лечения        (+10 HP) - 8 GOLD
2. Серебряный кинжал    (+5 ATK) - 10 GOLD
3. Попросить совета
4. Уйти
> 4

--------------------------------------------------
Деревня Угли
HP: 20/20   ATK: 4   GOLD: 10
--------------------------------------------------

Подойдя ближе к деревне вы видите
много разных людей, начиная от детей
заканчивая стариками, сидящими
молча возле главного центрального костра...

1. Идти к Лавка загадочного торговца
2. Идти к Берег синего моря
3. Инвентарь
>


*/