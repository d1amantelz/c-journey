#pragma once

#define MAX_LENGTH_NAME 96
#define MAX_DESC_LEN 512
#define MAX_EXITS 4
#define MAX_ACTIONS 5
#define MAX_BUFFER_SIZE 512

typedef struct
{
    int chestOpened;
    int cryptUnlocked;
    int bossDefeated;
} Quests;

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
    Quests quests;
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
