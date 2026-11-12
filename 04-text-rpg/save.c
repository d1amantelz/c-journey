#include "save.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void saveGame(Player *player)
{
    FILE *file = fopen("save.txt", "w");

    if (file == NULL)
    {
        typePrintf(20, "\nОшибка: не удалось создать файл сохранения save.txt!\n\n");
        return;
    }

    fprintf(file, "[PLAYER]\n");
    fprintf(file, "name=%s\n", player->name);
    fprintf(file, "loc=%d\n", player->currentLocId);
    fprintf(file, "hp=%d\n", player->hp);
    fprintf(file, "max_hp=%d\n", player->max_hp);
    fprintf(file, "base_attack=%d\n", player->base_attack);
    fprintf(file, "attack=%d\n", player->attack);
    fprintf(file, "gold=%d\n", player->gold);

    fprintf(file, "[QUESTS]\n");
    fprintf(file, "chest=%d\n", player->quests.chestOpened);
    fprintf(file, "crypt=%d\n", player->quests.cryptUnlocked);
    fprintf(file, "boss=%d\n", player->quests.bossDefeated);

    fprintf(file, "[INVENTORY]\n");
    fprintf(file, "count=%d\n", player->inventory.numItems);
    for (int i = 0; i < player->inventory.numItems; i++)
    {
        Item *item = &player->inventory.data[i];
        fprintf(file, "%d|%s|%s|%d|%d|%d\n", item->id, item->icon, item->name, item->attack, item->hp, item->cost);
    }

    fclose(file);
    typePrintf(20, "\n💾 Игра успешно сохранена в " COLOR_GREEN "save.txt" COLOR_RESET "!\n\n");
}

int loadGame(Player *player)
{
    FILE *file = fopen("save.txt", "r");

    if (file == NULL)
    {
        return 0;
    }

    char line[MAX_BUFFER_SIZE];

    if (player->inventory.data != NULL)
    {
        free(player->inventory.data);
        player->inventory.data = NULL;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "name=", 5) == 0)
        {
            sscanf(line + 5, "%95[^\r\n]", player->name);
        }
        else if (strncmp(line, "loc=", 4) == 0)
        {
            int loc = 0;
            sscanf(line + 4, "%d", &loc);
            player->currentLocId = (LocationId)loc;
        }
        else if (strncmp(line, "hp=", 3) == 0)
        {
            sscanf(line + 3, "%d", &player->hp);
        }
        else if (strncmp(line, "max_hp=", 7) == 0)
        {
            sscanf(line + 7, "%d", &player->max_hp);
        }
        else if (strncmp(line, "base_attack=", 12) == 0)
        {
            sscanf(line + 12, "%d", &player->base_attack);
        }
        else if (strncmp(line, "attack=", 7) == 0)
        {
            sscanf(line + 7, "%d", &player->attack);
        }
        else if (strncmp(line, "gold=", 5) == 0)
        {
            sscanf(line + 5, "%d", &player->gold);
        }

        else if (strncmp(line, "chest=", 6) == 0)
        {
            sscanf(line + 6, "%d", &player->quests.chestOpened);
        }
        else if (strncmp(line, "crypt=", 6) == 0)
        {
            sscanf(line + 6, "%d", &player->quests.cryptUnlocked);
        }
        else if (strncmp(line, "boss=", 5) == 0)
        {
            sscanf(line + 5, "%d", &player->quests.bossDefeated);
        }

        else if (strncmp(line, "count=", 6) == 0)
        {
            int count = 0;
            sscanf(line + 6, "%d", &count);

            player->inventory.numItems = 0;
            player->inventory.size = (count > 10) ? count + 5 : 10;
            player->inventory.data = malloc(player->inventory.size * sizeof(Item));

            if (player->inventory.data == NULL)
            {
                printf("\nОшибка выделения памяти при загрузке инвентаря!\n");
                fclose(file);
                return 0;
            }

            for (int i = 0; i < count; i++)
            {
                if (fgets(line, sizeof(line), file))
                {
                    Item item;
                    if (sscanf(line, "%d|%15[^|]|%95[^|]|%d|%d|%d", &item.id, item.icon, item.name, &item.attack, &item.hp, &item.cost) == 6)
                    {
                        player->inventory.data[player->inventory.numItems++] = item;
                    }
                }
            }
        }
    }

    fclose(file);
    return 1;
}
