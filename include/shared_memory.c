//
// Created by Bib's on 05-08-25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared_memory.h"

#define IPC_RESULT_ERROR (-1)

// Fonction interne : retourne un identifiant de segment partagé à partir d’un fichier
static int get_shared_block(char *filename, int size) {
    key_t key = ftok(filename, 0);
    printf("key-->%i",key);
    if (key == IPC_RESULT_ERROR) {
        return IPC_RESULT_ERROR;
    }

    return shmget(key, size, 0644 | IPC_CREAT);
}

// Attache un segment mémoire à ce processus
char *attach_memory_block(char *filename, int size) {
    int shared_block_id = get_shared_block(filename, size);
    char *result;

    if (shared_block_id == IPC_RESULT_ERROR) {
        return NULL;
    }

    result = shmat(shared_block_id, NULL, 0);
    if (result == (char *)IPC_RESULT_ERROR) {
        return NULL;
    }

    return result;
}

// Détache le segment mémoire du processus courant
bool detach_memory_block(char *block) {
    return (shmdt(block) != IPC_RESULT_ERROR);
}

// Supprime le segment mémoire du système
bool destroy_memory_block(char *filename) {
    int shared_block_id = get_shared_block(filename, 0);

    if (shared_block_id == IPC_RESULT_ERROR) {
        return false;
    }

    return (shmctl(shared_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}

