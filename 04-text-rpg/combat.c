#include "utils.h"
#include "types.h"
#include "inventory.h"
#include "combat.h"
#include <stdlib.h>

void handleBattle(Player *player)
{

    Enemy enemy = {"Гоблин", 10, 10, 3, 5, {311, "🗝️ ", "Ржавый ключ", 0, 0, 10}, 1};
    startBattle(player, &enemy);
}

int startBattle(Player *player, Enemy *enemy)
{
    typePrintf(7, COLOR_GRAY "\n======================================================================\n" COLOR_RESET);
    typePrintf(25, COLOR_RED "\nНАЧАЛСЯ БОЙ" COLOR_RESET ": " COLOR_MAGENTA "%s" COLOR_RESET " VS " COLOR_GREEN "%s" COLOR_RESET "!\n", player->name, enemy->name);

    char playerBar[128];
    char enemyBar[128];

    while (player->hp > 0 && enemy->hp > 0)
    {
        getHpBar(playerBar, player->hp, player->max_hp, 10);
        getHpBar(enemyBar, enemy->hp, enemy->max_hp, 10);
        typePrintf(25, "\n" COLOR_MAGENTA "%s" COLOR_RESET ": " COLOR_RED "%s" COLOR_RESET " " COLOR_RED "%d" COLOR_RESET "/" COLOR_RED "%d" COLOR_RESET " HP\n" COLOR_GREEN "%s" COLOR_RESET ": " COLOR_RED "%s" COLOR_RESET " " COLOR_RED "%d" COLOR_RESET "/" COLOR_RED "%d" COLOR_RESET " HP\n\n",
                   player->name,
                   playerBar,
                   player->hp,
                   player->max_hp,
                   enemy->name,
                   enemyBar,
                   enemy->hp,
                   enemy->max_hp);

        typePrintf(25, "1. ⚔️  Ударить оружием\n", player->attack);
        typePrintf(25, "2. 🎒 Открыть инвентарь\n");
        typePrintf(25, "3. 🏃 Попытаться сбежать\n> ");

        int choice = getInt();

        if (choice == 1)
        {
            enemy->hp -= player->attack;
            typePrintf(25, "\n⚔️  Вы атакуете! %s получает %d урона.\n", enemy->name, player->attack);

            if (enemy->hp <= 0)
            {
                typePrintf(7, COLOR_GRAY "\n======================================================================\n" COLOR_RESET);
                typePrintf(25, COLOR_GREEN "\nПОБЕДА" COLOR_RESET "! %s повержен!\n", enemy->name);
                typePrintf(25, "\n🪙" COLOR_YELLOW "  +%d" COLOR_RESET " золотых монет!\n", enemy->goldReward);
                player->gold += enemy->goldReward;

                if (enemy->hasDrop)
                {
                    typePrintf(25, "🎁 С поверженного врага упал трофей: \"%s\"!\n\n", enemy->dropItem.name);
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
                typePrintf(25, "\nВы ловко увернулись и сбежали из боя!\n");
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
    if (player->quests.bossDefeated)
    {
        typePrintf(25, "\nСреди разломанных костей и старых лат царит мертвая тишина.\nХранитель склепа уже повержен.\n\n");
        return;
    }

    Enemy boss = {"Хранитель склепа", 25, 25, 9, 30, {401, "⚔️ ", "Древний стальной меч", 8, 0, 50}, 1};
    if (startBattle(player, &boss))
    {
        player->quests.bossDefeated = 1;
        typePrintf(25, "Призрачный огонь в глазницах великана угас. Склеп очищен от древней тьмы!\n");
        typePrintf(25, "\n* Ваша базовая атака повысилась с %d до %d *\n\n", player->base_attack, player->base_attack + 2);
        player->base_attack += 2;
        player->attack += 2;
    }
}
