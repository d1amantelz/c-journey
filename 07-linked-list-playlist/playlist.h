#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stddef.h>

// Узел музыкального трека в связном списке
typedef struct Track {
    char title[64];             // Название песни
    char artist[64];            // Исполнитель
    int duration_sec;           // Длительность в секундах
    struct Track *next;         // Указатель на следующий трек в куче (или NULL)
} Track;

// Управляющий дескриптор плейлиста
typedef struct {
    Track *head;                // Голова списка (указатель на первый трек)
    size_t size;                // Текущее количество треков
} Playlist;

#endif // PLAYLIST_H
