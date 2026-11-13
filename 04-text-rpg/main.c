#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "utils.h"
#include "inventory.h"
#include "combat.h"
#include "world.h"
#include "save.h"
#include <time.h>

int main()
{
    srand(time(NULL));
    Location world[LOC_COUNT];
    initWorld(world);

    typePrintf(7, COLOR_GRAY "\n======================================================================\n" COLOR_RESET);
    typePrintf(7, "               🌙 " COLOR_CYAN "ОТБЛЕСК ЛУННОГО СВЕТА: НАЧАЛО ПУТИ" COLOR_RESET " 🌙\n");
    typePrintf(7, COLOR_GRAY "======================================================================\n\n" COLOR_RESET);

    Player player;
    player.inventory.data = NULL;

    int running = 1;
    int flag = running;

    typePrintf(25, "\n1. 📜 Новая игра\n2. 💾 Загрузить игру\n> ");
    int choice = getInt();
    int isLoaded = 0;

    if (choice == 2)
    {
        if (loadGame(&player))
        {
            isLoaded = 1;
            typePrintf(20, "\nС возвращением в игру, " COLOR_MAGENTA "%s" COLOR_RESET "!\n", player.name);
        }
        else
        {
            typePrintf(20, "\nСохранение не найдено! Начинаем новую игру...\n");
        }
    }

    if (!isLoaded)
    {
        char heroName[MAX_LENGTH_NAME];
        typePrint("\nВведите имя героя: > ", 25);
        getString(heroName, MAX_LENGTH_NAME);

        if (!initPlayer(&player, heroName))
        {
            return 1;
        }

        typePrintf(20, "\nДобро пожаловать, " COLOR_MAGENTA "%s" COLOR_RESET "!\nТы очнулся на берегу синего моря.\n", player.name);
        typePrintf(20, "Теплая волна омывает твои ноги.\nТы медленно встаешь и рассматриваешь\nместо вокруг...\n");
    }

    while (flag)
    {
        Location *loc = &world[player.currentLocId];
        running = 1;
        char hpBar[128];
        getHpBar(hpBar, player.hp, player.max_hp, 10);

        typePrintf(7, COLOR_GRAY "\n----------------------------------------------------------------------\n" COLOR_RESET);
        typePrintf(7, "%s " COLOR_CYAN "%s" COLOR_RESET " \n❤️  HP: " COLOR_RED "%s " COLOR_RED "%d" COLOR_RESET "/" COLOR_RED "%d" COLOR_RESET " | ⚔️  ATK: " COLOR_BLUE "%d" COLOR_RESET " | 🪙  GOLD: " COLOR_YELLOW "%d" COLOR_RESET "\n",
                   loc->icon, loc->name, hpBar, player.hp, player.max_hp, player.attack, player.gold);
        typePrintf(7, COLOR_GRAY "----------------------------------------------------------------------\n" COLOR_RESET);

        typePrintf(20, "\n%s\n\n", loc->description);

        while (running)
        {

            int option = 1;

            // Печать путей из текущей локации
            for (int i = 0; i < loc->exitsCount; i++)
            {
                LocationId targetId = loc->exits[i];
                typePrintf(20, "%d. " COLOR_GREEN "─>" COLOR_RESET " %s %s\n", option++, world[targetId].name, world[targetId].icon);
            }

            // Печать действия текущей локации
            for (int i = 0; i < loc->actionsCount; i++)
            {
                typePrintf(20, "%d. %s %s\n", option++, loc->actions[i].icon, loc->actions[i].name);
            }

            int invOption = option++;
            int saveOption = option++;

            // Печать системных действий
            typePrintf(20, "%d. 🎒 Открыть инвентарь \n", invOption);
            typePrintf(20, "%d. 💾 Сохранить игру \n", saveOption);
            typePrintf(20, "0. 🚪 Выход из игры\n> ");

            int choice = getInt();

            if (choice == 0)
            {
                typePrintf(20, "\n🌙 Вы легли отдохнуть на землю. До встречи!\n");
                running = 0;
                flag = 0;
                saveGame(&player);
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
            else if (choice == saveOption)
            {
                saveGame(&player);
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
