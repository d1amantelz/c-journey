#include "utils.h"
#include "types.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

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

void typePrint(const char *text, int delayMs)
{
    (void)delayMs;
    while (*text)
    {
        if (*text == '\033')
        {
            while (*text && *text != 'm')
            {
                putchar(*text++);
            }

            if (*text == 'm')
            {
                putchar(*text++);
            }

            fflush(stdout);
            continue;
        }

        putchar(*text);
        fflush(stdout);
        usleep(3 * 1000);
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

    typePrint(buffer, delayMs);
}

void sleepMs(int ms)
{
    usleep(ms * 1000);
}

void getHpBar(char *buffer, int current, int max, int barLength)
{
    if (max <= 0)
        max = 1;
    if (current < 0)
        current = 0;
    if (current > max)
        current = max;

    int filled = (current * barLength) / max;
    if (current > 0 && filled == 0)
    {
        filled = 1;
    }

    int idx = 0;

    idx += sprintf(buffer + idx, "%s[%s", COLOR_GRAY, COLOR_RED);

    for (int i = 0; i < filled; i++)
    {
        idx += sprintf(buffer + idx, "■");
    }

    idx += sprintf(buffer + idx, "%s", COLOR_GRAY);

    for (int i = filled; i < barLength; i++)
    {
        idx += sprintf(buffer + idx, "·");
    }

    idx += sprintf(buffer + idx, "]%s", COLOR_RESET);
}
