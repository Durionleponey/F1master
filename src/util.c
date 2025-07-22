#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>

int printLogo(void) {
    clear(); // nettoie l'écran (équivalent de system("clear"))

    printw("  ______ __   __  __           _            \n");
    printw(" |  ____/_ | |  \\/  |         | |           \n");
    printw(" | |__   | | | \\  / | __ _ ___| |_ ___ _ __ \n");
    printw(" |  __|  | | | |\\/| |/ _` / __| __/ _ \\ '__|\n");
    printw(" | |     | | | |  | | (_| \\__ \\ ||  __/ |   \n");
    printw(" |_|     |_| |_|  |_|\\__,_|___/\\__\\___|_|   by Bib's\n");
    printw("                                            \n");
    printw("                                            \n");

    refresh(); // met à jour l'affichage
    return 0;
}

