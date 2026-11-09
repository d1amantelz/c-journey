#pragma once
#include "types.h"

int initPlayer(Player *player, const char *name);

void freePlayer(Player *player);

void initWorld(Location world[]);

void handleAction(Player *player, ActionId act);

void openShop(Player *player);

void openShoreChest(Player *player);

void enterCrypt(Player *player);