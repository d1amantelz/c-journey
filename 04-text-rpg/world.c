#include "world.h"
#include "types.h"
#include "utils.h"
#include "inventory.h"
#include "combat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    player->quests.bossDefeated = 0;
    player->quests.chestOpened = 0;
    player->quests.cryptUnlocked = 0;

    if (player->inventory.data == NULL)
    {
        printf("\nОшибка выделения памяти под инвентарь!\n");
        return 0;
    }

    Item dagger = {1, "🗡️ ", "Ржавый кинжал", 1, 0, 3};
    Item bread = {2, "🥖", "Хлеб", 0, 3, 2};
    Item vodka = {3, "🍶", "Водка", 0, -1, 4};

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
           COLOR_GRAY "Вы видите бескрайний океан, освещенный яркой полной луной.\n"
                      "В стороне виден дым из труб и огонь костров. Видимо, это Деревня..." COLOR_RESET);
    world[LOC_SHORE].exitsCount = 1;
    world[LOC_SHORE].exits[0] = LOC_VILLAGE;
    world[LOC_SHORE].actionsCount = 1;
    strcpy(world[LOC_SHORE].actions[0].name, "Осмотреть выброшенный сундук");
    world[LOC_SHORE].actions[0].actionId = ACT_SHORE_CHEST;

    // Локация: Деревня Угли
    world[LOC_VILLAGE].id = LOC_VILLAGE;
    strcpy(world[LOC_VILLAGE].name, "Деревня Угли");
    strcpy(world[LOC_VILLAGE].description,
           COLOR_GRAY "В деревне вы видите много разных людей, сидящих молча\n"
                      "возле главного центрального костра..." COLOR_RESET);
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
           COLOR_GRAY "Зайдя в лавку, первое что вы увидели — это загадочный мужчина\n"
                      "в капюшоне, играющий в шахматы сам с собой..." COLOR_RESET);
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
           COLOR_GRAY "Пробираясь через лощину густых елок вы попадаете\n"
                      "на еле освещенную лунным светом поляну.\n"
                      "Вдруг вы слышите треск веток позади себя...\n"
                      "Приглядевшись вы увидели страшного низкого гоблина с острым осколком бутылки в руке..." COLOR_RESET);
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
           COLOR_GRAY "Леденящий сырой холод пробирает до костей.\n"
                      "В полумраке зала высится массивный каменный саркофаг,\n"
                      "покрытый древними рунами и трещинами. Воздух пахнет тленом и вековой пылью.\n"
                      "В глубине склепа слышится глухое бряцание тяжелых стальных лат..." COLOR_RESET);
    world[LOC_CRYPT].exitsCount = 1;
    world[LOC_CRYPT].exits[0] = LOC_DARK_FOREST;

    // Действия для локации: Склеп
    world[LOC_CRYPT].actionsCount = 1;
    strcpy(world[LOC_CRYPT].actions[0].name, "Бросить вызов Хранителю склепа");
    world[LOC_CRYPT].actions[0].actionId = ACT_CRYPT_BOSS_FIGHT;

    world[LOC_SHORE].icon = "🌊";
    world[LOC_VILLAGE].icon = "🔥";
    world[LOC_SHOP].icon = "🛒";
    world[LOC_DARK_FOREST].icon = "🌲";
    world[LOC_CRYPT].icon = "🏛️ ";

    world[LOC_SHORE].actions[0].icon = "📦";
    world[LOC_VILLAGE].actions[0].icon = "🔥";
    world[LOC_SHOP].actions[0].icon = "💬";
    world[LOC_SHOP].actions[1].icon = "🛒";
    world[LOC_DARK_FOREST].actions[0].icon = "⚔️ ";
    world[LOC_DARK_FOREST].actions[1].icon = "🗝️ ";
    world[LOC_CRYPT].actions[0].icon = "💀";
}

void handleAction(Player *player, ActionId act)
{

    switch (act)
    {
    case ACT_SHOP_ADVICE:
        printMerchantPhrases(player);
        // typePrintf(20, "\nТорговец:\n- Знаешь, путник... Не советовал бы я тебе бродить здесь ночью.\n\n");
        break;
    case ACT_REST_FIRE:
        typePrintf(20, "\n🏕️  Вы греетесь у костра. Тепло разливается по телу: " COLOR_RED "+5 ❤️ \n\n" COLOR_RESET);
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

void openShop(Player *player)
{
    Item shopItems[] = {
        {11, "🧪", "Зелье лечения", 0, 10, 7},
        {12, "🗡️ ", "Стальной кинжал", 5, 0, 15},
        {13, "💧", "Святая вода", 0, 5, 7},
    };
    int shopCount = 3;

    typePrintf(25, "\nТовары в наличии:\n");
    for (int i = 0; i < shopCount; i++)
    {
        typePrintf(25, "%d. %s %s · " COLOR_YELLOW "%d 🪙  GOLD · " COLOR_RESET, i + 1, shopItems[i].icon, shopItems[i].name, shopItems[i].cost);
        if (shopItems[i].attack)
        {
            typePrintf(25, COLOR_BLUE "%+d ⚔️  ATK\n" COLOR_RESET, shopItems[i].attack);
        }
        else if (shopItems[i].hp)
        {
            typePrintf(25, COLOR_RED "%+d ❤️  HP\n" COLOR_RESET, shopItems[i].hp);
        }
        else
        {
            printf("\n");
        }
    }

    typePrintf(25, "0. " COLOR_GREEN "<─" COLOR_RESET "  Назад\n> ");
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

    if (player->quests.chestOpened)
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
        typePrintf(25, COLOR_YELLOW "\n🪙  +15 золотых монет!\n" COLOR_RESET);

        player->gold += 15;
        Item amulet = {21, "💍", "Морское кольцо", 0, 15, 30};
        typePrintf(25, "\nВы также нашли старинное «%s» (+15 HP)!\n\n", amulet.name);

        addToInventory(&player->inventory, amulet);
        player->quests.chestOpened = 1;
    }
    else
    {
        typePrintf(25, "\nЗамок издал глухой скрежет... Код не подошел!\n\n");
    }
}

void enterCrypt(Player *player)
{
    if (!player->quests.cryptUnlocked)
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
        player->quests.cryptUnlocked = 1;
    }

    typePrintf(25, "\nВы спускаетесь по замшелым каменным ступеням в темноту склепа...\n");
    player->currentLocId = LOC_CRYPT;
}

void printMerchantPhrases(Player *player)
{
    if (player->quests.bossDefeated)
    {
        typePrintf(25, "\n" COLOR_GREEN "Торговец" COLOR_RESET ":\n— Невероятно! Ты одолел Хранителя склепа?! Во тьме снова засиял лунный свет... Деревня в долгу перед тобой, " COLOR_MAGENTA "%s" COLOR_RESET "!\n\n", player->name);
    }
    else if (player->quests.cryptUnlocked || findItem(&player->inventory, "Ржавый ключ") != -1)
    {
        typePrintf(25, "\n" COLOR_GREEN "Торговец" COLOR_RESET ":\n— Вижу, ты подобрался к тайнам склепа... Не суйся туда без хорошего оружия и пары зелий, путник!\n\n");
    }
    else
    {
        typePrintf(25, "\n" COLOR_GREEN "Торговец" COLOR_RESET ":\n— В Тёмном лесу шныряет гоблин... Местные говорят, он стащил ключ от древнего склепа.\n\n");
    }
}