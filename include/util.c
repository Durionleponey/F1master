#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>

int printLogo(void) {
    clear();

    printw("  ______ __   __  __           _            \n");
    printw(" |  ____/_ | |  \\/  |         | |           \n");
    printw(" | |__   | | | \\  / | __ _ ___| |_ ___ _ __ \n");
    printw(" |  __|  | | | |\\/| |/ _` / __| __/ _ \\ '__|\n");
    printw(" | |     | | | |  | | (_| \\__ \\ ||  __/ |   \n");
    printw(" |_|     |_| |_|  |_|\\__,_|___/\\__\\___|_|   \n");
    printw("                                            \n");
    printw("                                            \n");

    refresh();
    return 0;
}

int printLogo2(void) {
    system("clear");

    printf("  ______ __   __  __           _            \n");
    printf(" |  ____/_ | |  \\/  |         | |           \n");
    printf(" | |__   | | | \\  / | __ _ ___| |_ ___ _ __ \n");
    printf(" |  __|  | | | |\\/| |/ _` / __| __/ _ \\ '__|\n");
    printf(" | |     | | | |  | | (_| \\__ \\ ||  __/ |   \n");
    printf(" |_|     |_| |_|  |_|\\__,_|___/\\__\\___|_|  \n");
    printf("                                            \n");
    printf("                                            \n");

    return 0;
}
