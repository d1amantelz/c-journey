#pragma once
#include "types.h"

void addToInventory(Inventory *inventory, Item item);

void showInventory(const Inventory *inventory);

void removeFromInventory(Inventory *inventory, int index);

void openInventory(Player *player);

void useItem(Player *player, int index);

int findItem(Inventory *inventory, const char *name);
