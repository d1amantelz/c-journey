#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#define MAX_LENGTH_NAME 96
#define MAX_DESC_LEN 512
#define MAX_EXITS 4
#define MAX_ACTIONS 5
#define MAX_BUFFER_SIZE 512

typedef enum
{
    LOC_SHORE,
    LOC_VILLAGE,
    LOC_SHOP,
    LOC_CRYPT,
    LOC_DARK_FOREST,
    LOC_COUNT,
} LocationId;

typedef enum
{
    ACT_NONE,
    ACT_SHOP_ADVICE,
    ACT_SHOP_BUY,
    ACT_REST_FIRE,
    ACT_SHORE_CHEST,
    ACT_ENTER_CRYPT,
    ACT_CRYPT_BOSS_FIGHT,
    ACT_DARK_FOREST_FIGHT,
} ActionId;

typedef struct
{
    char name[MAX_LENGTH_NAME];
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
    LocationId currentLocId;
    int hp;
    int max_hp;
    int base_attack;
    int attack;
    int gold;
    Inventory inventory;
} Player;

typedef struct
{
    char name[MAX_LENGTH_NAME];
    int hp;
    int max_hp;
    int attack;
    int goldReward;
    Item dropItem;
    int hasDrop;
} Enemy;

typedef struct
{
    LocationId id;
    char name[MAX_LENGTH_NAME];
    char description[MAX_DESC_LEN];

    LocationId exits[MAX_EXITS];
    int exitsCount;

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

void openShop(Player *player);

void openShoreChest(Player *player);

void handleBattle(Player *player);

int startBattle(Player *player, Enemy *enemy);

int findItem(Inventory *inventory, const char *name);

void enterCrypt(Player *player);

void handleCryptBoss(Player *player);

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

    int running = 1;
    int flag = running;

    typePrintf(20, "\nДобро пожаловать, %s!\nТы очнулся на берегу синего моря.\n", player.name);
    typePrintf(20, "Теплая волна омывает твои ноги.\nТы медленно встаешь и рассматриваешь\nместо вокруг...\n");

    while (flag)
    {
        Location *loc = &world[player.currentLocId];
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
                player.currentLocId = loc->exits[choice - 1];
                running = 0;
            }
            else if (choice > loc->exitsCount && choice <= loc->exitsCount + loc->actionsCount)
            {
                int actionIndex = choice - loc->exitsCount - 1;
                LocationId oldLoc = player.currentLocId;
                handleAction(&player, loc->actions[actionIndex].actionId);

                if (player.currentLocId != oldLoc)
                {
                    running = 0;
                }
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
        player->attack = player->base_attack + item.attack;

        typePrintf(25, "\nВы экипировали: %s. ATK: %+d\n\n", item.name, item.attack);
    }
    else if (item.hp != 0)
    {

        if (item.hp > 0 && player->hp >= player->max_hp)
        {
            typePrintf(25, "\nВаше здоровье уже максимальное: %d / %d\n\n", player->hp, player->max_hp);
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

    player->currentLocId = LOC_SHORE;
    player->hp = 20;
    player->max_hp = 20;
    player->base_attack = 3;
    player->attack = player->base_attack;
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
    world[LOC_SHORE].actionsCount = 1;
    strcpy(world[LOC_SHORE].actions[0].name, "Осмотреть выброшенный сундук");
    world[LOC_SHORE].actions[0].actionId = ACT_SHORE_CHEST;

    // Локация: Деревня Угли
    world[LOC_VILLAGE].id = LOC_VILLAGE;
    strcpy(world[LOC_VILLAGE].name, "Деревня Угли");
    strcpy(world[LOC_VILLAGE].description,
           "В деревне вы видите много разных людей, сидящих молча\n"
           "возле главного центрального костра...");
    world[LOC_VILLAGE].exitsCount = 3;
    world[LOC_VILLAGE].exits[0] = LOC_SHORE;
    world[LOC_VILLAGE].exits[1] = LOC_SHOP;
    world[LOC_VILLAGE].exits[2] = LOC_DARK_FOREST;

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
    world[LOC_SHOP].actionsCount = 2;
    strcpy(world[LOC_SHOP].actions[0].name, "Попросить совета");
    strcpy(world[LOC_SHOP].actions[1].name, "Посмотреть товары");
    world[LOC_SHOP].actions[0].actionId = ACT_SHOP_ADVICE;
    world[LOC_SHOP].actions[1].actionId = ACT_SHOP_BUY;

    // Локация: Темный лес
    world[LOC_DARK_FOREST].id = LOC_DARK_FOREST;
    strcpy(world[LOC_DARK_FOREST].name, "Темный лес");
    strcpy(world[LOC_DARK_FOREST].description,
           "Пробираясь через лощину густых елок вы попадаете\n"
           "на еле освещенную лунным светом поляну.\n"
           "Вдруг вы слышите треск веток позади себя...\n"
           "Приглядевшись вы увидели страшного низкого гоблина с острым осколком бутылки в руке...");
    world[LOC_DARK_FOREST].exitsCount = 1;
    world[LOC_DARK_FOREST].exits[0] = LOC_VILLAGE;

    // Действия для локации: Темный лес
    world[LOC_DARK_FOREST].actionsCount = 2;
    strcpy(world[LOC_DARK_FOREST].actions[0].name, "Сразиться с гоблином");
    strcpy(world[LOC_DARK_FOREST].actions[1].name, "Войти в склеп");
    world[LOC_DARK_FOREST].actions[0].actionId = ACT_DARK_FOREST_FIGHT;
    world[LOC_DARK_FOREST].actions[1].actionId = ACT_ENTER_CRYPT;

    // Локация: Склеп
    world[LOC_CRYPT].id = LOC_CRYPT;
    strcpy(world[LOC_CRYPT].name, "Заброшенный склеп");
    strcpy(world[LOC_CRYPT].description,
           "Леденящий сырой холод пробирает до костей.\n"
           "В полумраке зала высится массивный каменный саркофаг,\n"
           "покрытый древними рунами и трещинами. Воздух пахнет тленом и вековой пылью.\n"
           "В глубине склепа слышится глухое бряцание тяжелых стальных лат...");
    world[LOC_CRYPT].exitsCount = 1;
    world[LOC_CRYPT].exits[0] = LOC_DARK_FOREST;

    // Действия для локации: Склеп
    world[LOC_CRYPT].actionsCount = 1;
    strcpy(world[LOC_CRYPT].actions[0].name, "Бросить вызов Хранителю склепа");
    world[LOC_CRYPT].actions[0].actionId = ACT_CRYPT_BOSS_FIGHT;
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
    case ACT_SHOP_BUY:
        openShop(player);
        break;
    case ACT_SHORE_CHEST:
        openShoreChest(player);
        break;
    case ACT_DARK_FOREST_FIGHT:
        handleBattle(player);
        break;
    case ACT_ENTER_CRYPT:
        enterCrypt(player);
        break;
    case ACT_CRYPT_BOSS_FIGHT:
        handleCryptBoss(player);
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

    typePrint(buffer, 1);
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

void openShop(Player *player)
{
    // предметы для торговли. id, name, attack, hp, cost
    Item shopItems[] = {
        {11, "Зелье лечения", 0, 10, 7},
        {12, "Стальной кинжал", 5, 0, 15},
        {13, "Святая вода", 0, 5, 7},
    };
    int shopCount = 3;

    typePrintf(25, "\nТовары в наличии:\n");
    for (int i = 0; i < shopCount; i++)
    {
        typePrintf(25, "%d. %s | %d GOLD |", i + 1, shopItems[i].name, shopItems[i].cost);

        if (shopItems[i].attack)
        {
            typePrintf(25, " %+d ATK\n", shopItems[i].attack);
        }
        else if (shopItems[i].hp)
        {
            typePrintf(25, " %+d HP\n", shopItems[i].hp);
        }
        else
        {
            printf("\n");
        }
    }

    typePrintf(25, "0. Назад\n> ");
    int choice = getInt();

    if (choice == 0)
    {
        typePrintf(25, "\nТорговец:\n- Заходи когда будет время.\n\n");
        return;
    }

    if (choice < 1 || choice > shopCount)
    {
        typePrintf(25, "\nТорговец:\n- У меня нет такого предмета.\n\n");
        return;
    }

    Item chosenItem = shopItems[choice - 1];

    if (player->gold < chosenItem.cost)
    {
        typePrintf(25, "\nТорговец:\n- Недостаточно золота. Нужно еще %d монет.\n\n", chosenItem.cost - player->gold);
        return;
    }

    player->gold -= chosenItem.cost;
    addToInventory(&player->inventory, chosenItem);

    typePrintf(25, "\nВы купили \"%s\". Осталось монет: %d \n\n", chosenItem.name, player->gold);
}

void openShoreChest(Player *player)
{
    static int isOpened = 0;

    if (isOpened)
    {
        typePrintf(25, "\nСундук уже открыт!\n\n");
        return;
    }

    typePrintf(25, "\nВы подходите к тяжелому сундуку, выброшенному прибоем на мокрый песок.\n");
    typePrintf(25, "Крышка заперта кодовым замком из трех цифр.\n");
    typePrintf(25, "Рядом нацарапана загадка:\n");
    typePrintf(25, "«Первая цифра — число лун в ночном небе (1),\n"
                   " вторая — сколько дорог ведет из деревни (2),\n"
                   " третья — сколько вещей было в твоем кармане в начале пути (3)».\n\n");

    typePrintf(25, "Введите трехначный код (0 - уйти): > ");
    int choice = getInt();

    if (choice == 0)
    {
        typePrintf(25, "\nВы решили пока не испытывать удачу.\n\n");
        return;
    }

    if (choice == 123)
    {
        typePrintf(25, "\n*ЩЁЛК!* Старый латунный замок со скрипом поддается!\n");
        typePrintf(25, "Крышка откидывается. Внутри блестят сокровища:\n");
        typePrintf(25, "+15 золотых монет!\n");

        player->gold += 15;
        Item amulet = {21, "Морское кольцо", 0, 15, 30};
        typePrintf(25, "\nВы также нашли старинное «%s» (+15 HP)!\n\n", amulet.name);

        addToInventory(&player->inventory, amulet);
        isOpened = 1;
    }
    else
    {
        typePrintf(25, "\nЗамок издал глухой скрежет... Код не подошел!\n\n");
    }
}

void handleBattle(Player *player)
{

    Enemy enemy = {"Гоблин", 10, 10, 3, 5, {311, "Ржавый ключ", 0, 0, 10}, 1};
    startBattle(player, &enemy);
}

int startBattle(Player *player, Enemy *enemy)
{
    typePrintf(25, "\n======================================================================\n");
    typePrintf(25, "НАЧАЛСЯ БОЙ: %s VS %s!\n", player->name, enemy->name);
    typePrintf(25, "======================================================================\n");

    while (player->hp > 0 && enemy->hp > 0)
    {
        typePrintf(25, "\nВаше здоровье: %d / %d | Враг: %s (%d / %d HP)\n",
                   player->hp,
                   player->max_hp,
                   enemy->name,
                   enemy->hp,
                   enemy->max_hp);

        typePrintf(25, "1. Ударить оружием (%d ATK)\n", player->attack);
        typePrintf(25, "2. Открыть инвентарь\n");
        typePrintf(25, "3. Попытаться сбежать\n> ");

        int choice = getInt();

        if (choice == 1)
        {
            enemy->hp -= player->attack;
            typePrintf(25, "\nВы атакуете! %s получает %d урона.\n", enemy->name, player->attack);

            if (enemy->hp <= 0)
            {
                typePrintf(25, "\nПОБЕДА! %s повержен!\n", enemy->name);
                typePrintf(25, "Вы получили %d золотых монет!\n", enemy->goldReward);
                player->gold += enemy->goldReward;

                if (enemy->hasDrop)
                {
                    typePrintf(25, "С поверженного врага упал трофей: \"%s\"!\n\n", enemy->dropItem.name);
                    addToInventory(&player->inventory, enemy->dropItem);
                }

                return 1;
            }
        }
        else if (choice == 2)
        {
            openInventory(player);
            continue;
        }
        else if (choice == 3)
        {
            if (rand() % 2 == 0)
            {
                typePrintf(25, "\nВы ловко увернулись и сбежали из боя!\n\n");
                player->currentLocId = LOC_VILLAGE;
                return 0;
            }
            else
            {
                typePrintf(25, "\nПобег не удался! Враг заблокировал дорогу!\n");
            }
        }
        else
        {
            typePrintf(25, "\nВы замешкались...\n");
        }

        typePrintf(25, "%s свирепо атакует в ответ! Вы теряете %d HP.\n",
                   enemy->name,
                   enemy->attack);
        player->hp -= enemy->attack;

        if (player->hp <= 0)
        {
            typePrintf(25, "\nТьма сгущается перед глазами... Вас одолели в бою.\n");
            typePrintf(25, "Вы очнулись возле тёплого костра в Деревне Угли (HP: 1/%d).\n\n", player->max_hp);
            player->currentLocId = LOC_VILLAGE;
            player->hp = 1;
            return 0;
        }
    }

    return 0;
}

void handleCryptBoss(Player *player)
{
    static int isBossDefeated = 0;

    if (isBossDefeated)
    {
        typePrintf(25, "\nСреди разломанных костей и старых лат царит мертвая тишина.\nХранитель склепа уже повержен.\n\n");
        return;
    }

    Enemy boss = {"Хранитель склепа", 25, 25, 9, 30, {401, "Древний стальной меч", 8, 0, 50}, 1};
    if (startBattle(player, &boss))
    {
        isBossDefeated = 1;
        typePrintf(25, "Призрачный огонь в глазницах великана угас. Склеп очищен от древней тьмы!\n");
        typePrintf(25, "\n* Ваша базовая атака повысилась с %d до %d *\n\n", player->base_attack, player->base_attack + 2);
        player->base_attack += 2;
        player->attack += 2;
    }
}

int findItem(Inventory *inventory, const char *name)
{
    for (int i = 0; i < inventory->numItems; i++)
    {
        if (strcmp(inventory->data[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}

void enterCrypt(Player *player)
{
    static int isUnlocked = 0;

    if (!isUnlocked)
    {
        int keyIdx = findItem(&player->inventory, "Ржавый ключ");
        if (keyIdx == -1)
        {
            typePrintf(25, "\nМассивная железная дверь опутана цепями и заперта на замок.\n");
            typePrintf(25, "Замочная скважина покрыта мхом. Нужен подходящий ключ!\n\n");
            return;
        }

        typePrintf(25, "\nВы вставляете Ржавый ключ в замок... Щелчок! Дверь склепа со скрипом поддается!\n");
        removeFromInventory(&player->inventory, keyIdx);
        isUnlocked = 1;
    }

    typePrintf(25, "Вы спускаетесь по замшелым каменным ступеням в темноту склепа...\n\n");
    player->currentLocId = LOC_CRYPT;
}
