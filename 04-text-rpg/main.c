#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#define MAX_LENGTH_NAME 32
#define MAX_DESC_LEN 512
#define MAX_EXITS 4
#define MAX_ACTIONS 5
#define MAX_BUFFER_SIZE 512

typedef enum
{
    LOC_SHORE,
    LOC_VILLAGE,
    LOC_SHOP,
    LOC_COUNT,
} LocationId;

typedef enum
{
    ACT_NONE,
    ACT_SHOP_ADVICE,
    ACT_SHOP_BUY,
    ACT_REST_FIRE,
} ActionId;

typedef struct
{
    char name[48];
    ActionId actionId;
} Action;

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
    int hp;
    int max_hp;
    int attack;
    int gold;
    Inventory inventory;
} Player;

typedef struct
{
    LocationId id;
    char name[64];
    char description[MAX_DESC_LEN];

    LocationId exits[MAX_EXITS]; // массив путей локации куда можно пойти
    int exitsCount;              // кол-во путей

    Action actions[MAX_ACTIONS];
    int actionsCount;

} Location;

void addToInventory(Inventory *inventory, Item item);

void showInventory(const Inventory *inventory);

int initPlayer(Player *player, const char *name);

void freePlayer(Player *player);

void initWorld(Location world[]);

int getInt();

void getString(char str[], int max_len);

int utf8Len(char const *string);

void handleAction(Player *player, ActionId act);

void typePrint(const char *text, int delayMs);

void sleepMs(int ms);

void typePrintf(int delayMs, const char *format, ...);

void removeFromInventory(Inventory *inventory, int index);

void openInventory(Player *player);

void useItem(Player *player, int index);

int main()
{
    Location world[LOC_COUNT];
    initWorld(world);

    typePrintf(7, "\n======================================================================\n");
    typePrintf(7, "                ОТБЛЕСК ЛУННОГО СВЕТА: НАЧАЛО ПУТИ\n");
    typePrintf(7, "======================================================================\n\n");

    Player player;
    if (!initPlayer(&player, "Герой"))
    {
        return 1;
    }

    typePrint("Введите имя героя: > ", 25);
    getString(player.name, MAX_LENGTH_NAME);

    LocationId currentLocId = LOC_SHORE;
    int running = 1;
    int flag = running;

    typePrintf(20, "\nДобро пожаловать, %s!\nТы очнулся на берегу синего моря.\n", player.name);
    typePrintf(20, "Теплая волна омывает твои ноги.\nТы медленно встаешь и рассматриваешь\nместо вокруг...\n");

    while (flag)
    {
        Location *loc = &world[currentLocId];
        running = 1;

        typePrintf(7, "\n----------------------------------------------------------------------\n");
        typePrintf(7, "%s  \nHP: %d/%d | ATK: %d | GOLD: %d\n",
                   loc->name, player.hp, player.max_hp, player.attack, player.gold);
        typePrintf(7, "----------------------------------------------------------------------\n");

        typePrintf(20, "\n%s\n\n", loc->description);

        while (running)
        {

            int option = 1;

            // Печать путей из текущей локации
            for (int i = 0; i < loc->exitsCount; i++)
            {
                LocationId targetId = loc->exits[i];
                typePrintf(20, "%d. Идти в: %s\n", option++, world[targetId].name);
            }

            // Печать действия текущей локации
            for (int i = 0; i < loc->actionsCount; i++)
            {
                typePrintf(20, "%d. %s\n", option++, loc->actions[i].name);
            }

            int invOption = option++;

            // Печать системных действий
            typePrintf(20, "%d. Открыть инвентарь\n", invOption);
            typePrintf(20, "0. Выход из игры\n> ");

            int choice = getInt();

            if (choice == 0)
            {
                typePrintf(20, "\nВы легли отдохнуть на землю. До встречи!\n");
                running = 0;
                flag = 0;
            }
            else if (choice >= 1 && choice <= loc->exitsCount)
            {
                currentLocId = loc->exits[choice - 1];
                running = 0;
            }
            else if (choice > loc->exitsCount && choice <= loc->exitsCount + loc->actionsCount)
            {
                int actionIndex = choice - loc->exitsCount - 1;
                handleAction(&player, loc->actions[actionIndex].actionId);
            }
            else if (choice == invOption)
            {
                openInventory(&player);
            }
            else
            {
                typePrintf(20, "\nНеверный выбор! Попробуйте снова!\n");
            }
        }
    }

    freePlayer(&player);
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
        Item item = inventory->data[i];
        typePrintf(20, "  %d. %s", i + 1, item.name);

        int nameLength = utf8Len(item.name);

        for (int s = 0; s < (22 - nameLength); s++)
        {
            typePrintf(25, " ");
        }

        if (item.hp != 0)
        {
            typePrintf(20, "(%+d HP)\n", item.hp);
        }
        else if (item.attack != 0)
        {
            typePrintf(20, "(%+d ATK)\n", item.attack);
        }
        else
        {
            printf("\n");
        }
    }
}

void openInventory(Player *player)
{
    showInventory(&player->inventory);

    if (player->inventory.numItems == 0)
    {
        typePrintf(25, "\nИнвентарь пуст.\n");
        return;
    }

    typePrintf(25, "\n1. Использовать\n2. Назад\n> ");

    int choice = getInt();
    
    if (choice == 2)
    {
        printf("\n");
        return;
    }

    typePrintf(25, "\nВыберите номер предмета:\n> ");
    choice = getInt();

    if (choice < 1 || choice > player->inventory.numItems)
    {
        typePrintf(25, "\nНеправильный выбор предмета!\n");
        return;
    }

    useItem(player, choice - 1);
}

void useItem(Player *player, int index)
{

    Item item = player->inventory.data[index];

    if (item.attack > 0)
    {
        player->attack += item.attack;

        typePrintf(25, "\nВы экипировали: %s. ATK: %+d\n\n", item.name, item.attack);
    }
    else if (item.hp != 0)
    {

        if (item.hp > 0 && player->hp >= player->max_hp)
        {
            typePrintf(25, "\nВаше здоровье уже максимальное: %d / %d\n", player->hp, player->max_hp);
            return;
        }

        player->hp += item.hp;

        if (player->hp > player->max_hp)
            player->hp = player->max_hp;

        if (item.hp > 0)
        {
            typePrintf(25, "\nВы употребили \"%s\" (%+d HP). HP: %d / %d\n", item.name, item.hp, player->hp, player->max_hp);
        }
        else
        {
            typePrintf(25, "\nВы употребили \"%s\". Вам поплохело (%d HP). HP: %d / %d\n", item.name, item.hp, player->hp, player->max_hp);
        }

        removeFromInventory(&player->inventory, index);
    }
}

int initPlayer(Player *player, const char *name)
{
    strncpy(player->name, name, MAX_LENGTH_NAME - 1);
    player->name[MAX_LENGTH_NAME - 1] = '\0';

    player->hp = 20;
    player->max_hp = 20;
    player->attack = 3;
    player->gold = 10;

    player->inventory.numItems = 0;
    player->inventory.size = 10;
    player->inventory.data = malloc(player->inventory.size * sizeof(Item));

    if (player->inventory.data == NULL)
    {
        printf("\nОшибка выделения памяти под инвентарь!\n");
        return 0;
    }

    Item dagger = {1, "Ржавый кинжал", 1, 0, 3};
    Item bread = {2, "Хлеб", 0, 3, 2};
    Item vodka = {3, "Водка", 0, -1, 4};

    addToInventory(&player->inventory, dagger);
    addToInventory(&player->inventory, bread);
    addToInventory(&player->inventory, vodka);

    return 1;
}

void freePlayer(Player *player)
{
    free(player->inventory.data);
    player->inventory.data = NULL;
}

void initWorld(Location world[])
{

    // Локация: Берег синего моря
    world[LOC_SHORE].id = LOC_SHORE;
    strcpy(world[LOC_SHORE].name, "Берег синего моря");
    strcpy(world[LOC_SHORE].description,
           "Вы видите бескрайний океан, освещенный яркой полной луной.\n"
           "В стороне виден дым из труб и огонь костров. Видимо, это Деревня...");
    world[LOC_SHORE].exitsCount = 1;
    world[LOC_SHORE].exits[0] = LOC_VILLAGE;
    world[LOC_SHORE].actionsCount = 0;

    // Локация: Деревня Угли
    world[LOC_VILLAGE].id = LOC_VILLAGE;
    strcpy(world[LOC_VILLAGE].name, "Деревня Угли");
    strcpy(world[LOC_VILLAGE].description,
           "В деревне вы видите много разных людей, сидящих молча\n"
           "возле главного центрального костра...");
    world[LOC_VILLAGE].exitsCount = 2;
    world[LOC_VILLAGE].exits[0] = LOC_SHORE;
    world[LOC_VILLAGE].exits[1] = LOC_SHOP;

    // Действия для локации: Деревня
    world[LOC_VILLAGE].actionsCount = 1;
    strcpy(world[LOC_VILLAGE].actions[0].name, "Погреться у костра");
    world[LOC_VILLAGE].actions[0].actionId = ACT_REST_FIRE;

    // Локация: Лавка торговца
    world[LOC_SHOP].id = LOC_SHOP;
    strcpy(world[LOC_SHOP].name, "Лавка загадочного торговца");
    strcpy(world[LOC_SHOP].description,
           "Зайдя в лавку, первое что вы увидели — это загадочный мужчина\n"
           "в капюшоне, играющий в шахматы сам с собой...");
    world[LOC_SHOP].exitsCount = 1;
    world[LOC_SHOP].exits[0] = LOC_VILLAGE;

    // Действия для локации: Лавка торговца
    world[LOC_SHOP].actionsCount = 1;
    strcpy(world[LOC_SHOP].actions[0].name, "Попросить совета");
    world[LOC_SHOP].actions[0].actionId = ACT_SHOP_ADVICE;
}

int getInt()
{
    int value;
    scanf("%d", &value);
    while (getchar() != '\n')
        ;
    return value;
}

void getString(char str[], int max_len)
{
    fgets(str, max_len, stdin);
    str[strcspn(str, "\n")] = '\0';
}

int utf8Len(char const *string)
{
    int len = 0;
    while (*string)
    {
        if ((*string & 0xC0) != 0x80)
            len++;
        string++;
    }
    return len;
}

void handleAction(Player *player, ActionId act)
{

    switch (act)
    {
    case ACT_SHOP_ADVICE:
        typePrintf(20, "\nТорговец:\n- Знаешь, путник... Не советовал бы я тебе бродить здесь ночью.\n\n");
        break;
    case ACT_REST_FIRE:
        typePrintf(20, "\nВы греетесь у костра. Тепло разливается по телу (+5 HP).\n\n");
        player->hp = (player->hp + 5 > player->max_hp) ? player->max_hp : player->hp + 5;
        break;
    default:
        break;
    }
}

void typePrint(const char *text, int delayMs)
{
    while (*text)
    {
        putchar(*text);
        fflush(stdout);
        usleep(delayMs * 1000);
        text++;
    }
}

void typePrintf(int delayMs, const char *format, ...)
{
    char buffer[MAX_BUFFER_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    typePrint(buffer, delayMs);
}

void sleepMs(int ms)
{
    usleep(ms * 1000);
}

void removeFromInventory(Inventory *inventory, int index)
{
    if (index < 0 || index >= inventory->numItems)
    {
        return;
    }

    for (int i = index; i < inventory->numItems - 1; i++)
    {
        inventory->data[i] = inventory->data[i + 1];
    }

    inventory->numItems--;
}

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