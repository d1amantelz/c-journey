#pragma once

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[1;31m"     // HP, урон, враги, кровь
#define COLOR_GREEN "\033[1;32m"   // Зелья лечения, победа
#define COLOR_YELLOW "\033[1;33m"  // Золото, монеты, сундук
#define COLOR_BLUE "\033[1;34m"    // Волна, побережье, мана
#define COLOR_MAGENTA "\033[1;35m" // Босс склепа, редкие артефакты
#define COLOR_CYAN "\033[1;36m"    // Названия локаций, меню
#define COLOR_WHITE "\033[1;37m"   // Яркий текст
#define COLOR_GRAY "\033[0;90m"    // Рамки, разделительные полосы (---)

int getInt();

void getString(char str[], int max_len);

int utf8Len(char const *string);

void typePrint(const char *text, int delayMs);

void sleepMs(int ms);

void typePrintf(int delayMs, const char *format, ...);

void getHpBar(char *buffer, int current, int max, int barLength);
