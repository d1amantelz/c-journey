#include "inventory.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("\n🎒 Инвентарь (%d/%d):\n",
           inventory->numItems, inventory->size);

    for (int i = 0; i < inventory->numItems; i++)
    {
        Item item = inventory->data[i];
        typePrintf(20, "  %d. %s %s", i + 1, item.icon, item.name);

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

    typePrintf(25, "\n1. ✨ Использовать\n2. " COLOR_GREEN "<─" COLOR_RESET " Назад\n> ");

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

        typePrintf(25, "\nВы экипировали: %s %s. ATK: %+d\n\n", item.icon, item.name, item.attack);
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
            typePrintf(25, "\nВы употребили \"%s %s\" (%+d HP). HP: %d / %d\n\n", item.icon, item.name, item.hp, player->hp, player->max_hp);
        }
        else
        {
            typePrintf(25, "\nВы употребили \"%s %s\". Вам поплохело (%d HP). HP: %d / %d\n\n", item.icon, item.name, item.hp, player->hp, player->max_hp);
        }

        removeFromInventory(&player->inventory, index);
    }
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